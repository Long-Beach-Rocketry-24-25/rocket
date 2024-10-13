
#include "st_bxcan.h"

static inline uint32_t tir_mask(CanId id)
{
    uint32_t tir = 0;

    if (id.type == CAN_STANDARD_ID)
    {
        /*
         * Default ID type = standard.
         */

        /*
         * Set ID.
         */
        tir |= ((id.id & 0x7FF) << CAN_TI0R_STID_Pos);
    }
    else
    {
        /*
         * Extended ID.
         */
        tir |= CAN_TI0R_IDE;

        /*
         * Set ID (todo).
         */
    }

    return tir;
}

static inline uint32_t tx_status_mask(uint8_t tx_box)
{
    /*
     * CAN_TSR_RQCP0-2 = 0x1UL << 0/8/16.
     * CAN_TSR_TXOK0-2 = 0x1UL << 1/9/17.
     */
    return (0x3UL << (tx_box * 8));
}

static bool enter_init_mode(StPrivBxCan* can)
{
    /*
     * Set inrq - enter init mode.
     */
    can->instance->MCR |= CAN_MCR_INRQ;

    /*
     * Clear sleep.
     */
    can->instance->MCR &= ~(CAN_MCR_SLEEP);

    /*
     * Confirm INAK.
     */
    WAIT(can->timer, can->instance->MSR & CAN_MSR_INAK, false);

    return true;
}

static bool enter_normal_mode(StPrivBxCan *can)
{
    /*
     * If not INAK return false.
     */
    if (!(can->instance->MSR & CAN_MSR_INAK))
    {
        return false;
    }

    /*
     * Clear INRQ.
     */
    can->instance->MCR &= ~(CAN_MCR_INRQ);

    /*
     * Wait INAK/SLAK = 0;
     */
    WAIT(can->timer, !(can->instance->MSR & (CAN_MSR_INAK | CAN_MSR_SLAK)), false);

    return true;
}

bool set_bit_timing(StPrivBxCan *can, uint8_t sjw, uint8_t ts1, uint8_t ts2, uint16_t brp)
{
    /*
     * Return false if not in init mode.
     */
    if (!(can->instance->MSR & CAN_MSR_INAK))
    {
        return false;
    }

    /*
     * BTR - BRP, TS1, TS2, SJW.
     */
    can->instance->BTR &= ~(CAN_BTR_SJW | CAN_BTR_TS2 | CAN_BTR_TS1 | CAN_BTR_BRP);
    can->instance->BTR |= ((sjw << CAN_BTR_SJW_Pos) & CAN_BTR_SJW);
    can->instance->BTR |= ((ts2 << CAN_BTR_TS2_Pos) & CAN_BTR_TS2);
    can->instance->BTR |= ((ts1 << CAN_BTR_TS1_Pos) & CAN_BTR_TS1);
    can->instance->BTR |= (brp & CAN_BTR_BRP);

    return true;
}

static bool set_filters(StPrivBxCan *can)
{
    /*
     * WIP...
     */

    // Set start bank for CAN2 - l452 has no can2
    // can->FMR |= (14 << CAN_FMR);

    // FINIT in FMR or FACT to deactivate
    can->instance->FMR |= CAN_FMR_FINIT;

    // FM1R FBM default 0 = mask

    // FS1R single 32 bit scale
    can->instance->FS1R |= CAN_FS1R_FSC0;

    // Default assignment to FIFO 0

    // Accept all
    can->instance->sFilterRegister[0].FR1 = 0x0;

    // Activate
    can->instance->FA1R |= CAN_FA1R_FACT0;

    // Reactivate
    can->instance->FMR &= ~(CAN_FMR_FINIT);

    return true;
}

static bool set_mode(StPrivBxCan *can)
{
    /*
     * Return false if not in init mode.
     */
    if (!(can->instance->MSR & CAN_MSR_INAK))
    {
        return false;
    }

    /*
     * Set SILM/LBKM - WIP...
     */
    can->instance->BTR |= CAN_BTR_LBKM;
    can->instance->BTR |= CAN_BTR_SILM;
    return true;
}

bool St_BxCan_Init(CanBus *can, StBxCanParams *params, Timeout *timer)
{
    if (params->tx_box >= CAN_NUM_TX_MAILBOXES ||
        params->rx_box >= CAN_NUM_RX_MAILBOXES)
    {
        return false;
    }

    params->priv.id = params->id;
    params->priv.instance = (ST_CAN_TypeDef *) params->base_addr;
    params->priv.timer = timer;
    params->priv.tx_box = params->tx_box;
    params->priv.rx_box = params->rx_box;
    params->priv.remote = false;

    can->priv = (void *) &params->priv;
    can->send = St_BxCan_Send;
    can->recv = St_BxCan_Recv;
    can->set_id = St_BxCan_Set_Id;
}

void St_BxCan_Config(CanBus *can)
{
    StPrivBxCan *dev = (StPrivBxCan *) can->priv;

    dev->rx.config(&dev->rx);
    dev->tx.config(&dev->tx);

    enter_init_mode(dev);
    set_mode(dev);
    set_bit_timing(dev, 1, 15, 2, 4);
    set_filters(dev);
    enter_normal_mode(dev);

}

void St_BxCan_Set_Id(CanBus *can, CanId new_id)
{
    StPrivBxCan *dev = (StPrivBxCan *) can->priv;
    dev->id = new_id;
}

bool St_BxCan_Send(CanBus* can, uint8_t *data, size_t size)
{
    StPrivBxCan *dev = (StPrivBxCan *) can->priv;

    if (size > CAN_MAX_DATA_BYTES)
    {
        return false;
    }

    ST_CAN_TxMailBox_TypeDef *box = &dev->instance->sTxMailBox[dev->tx_box];

    /*
     * Return false if mailbox not empty.
     */
    if (box->TIR & CAN_TI0R_TXRQ)
    {
        return false;
    }

    /*
     * Set DLC.
     */
    box->TDTR &= ~CAN_TDT0R_DLC;
    box->TDTR |= ((size & 0xF) << CAN_TDT0R_DLC_Pos);

    uint32_t low = 0;
    uint32_t high = 0;

    for (uint8_t i = 0; i < size && i < CAN_MAX_DATA_BYTES; ++i)
    {
        if (i < 4)
        {
            low |= data[i] << (i * 8);
        }
        else
        {
            high |= data[i] << ((i - 4) * 8);
        }
    }

    /*
     * Load Tx data.
     */
    box->TDLR = low;
    box->TDHR = high;

    /*
     * Request Tx transaction.
     */
    uint32_t tir = tir_mask(dev->id);

    /*
     * Set RTR.
     */
    if (dev->remote)
    {
        tir |= CAN_TI0R_RTR;
    }
    else
    {
        tir &= ~CAN_TI0R_RTR;
    }

    tir |= CAN_TI0R_TXRQ;


    box->TIR = tir;

    uint32_t mask = tx_status_mask(dev->tx_box);
    WAIT(dev->timer, ((dev->instance->TSR & mask) == mask), false);

    return true;
}

bool St_BxCan_Recv(CanBus *can, uint8_t *data, size_t size)
{
    StPrivBxCan *dev = (StPrivBxCan *) can->priv;

    /*
     * No data ready.
     */
    if (!(dev->instance->RF0R & CAN_RF0R_FMP0))
    {
        return false;
    }

    ST_CAN_TxMailBox_TypeDef *box = &dev->instance->sFIFOMailBox[dev->rx_box];

    uint32_t low = box->TDLR;
    uint32_t high = box->TDHR;

    /*
     * Read mailbox content.
     */
    for (size_t i = 0; i < size && i < CAN_MAX_DATA_BYTES; ++i)
    {
        if (i < 4)
        {
            data[i] = (low >> (i * 8)) & 0xF;
        }
        else
        {
            data[i] = (high >> ((i - 4) * 8)) & 0xF;
        }
    }

    /*
     * Set RFOM = 1 to release mailbox.
     */
    dev->instance->RF0R |= CAN_RF0R_RFOM0;

    return true;
}