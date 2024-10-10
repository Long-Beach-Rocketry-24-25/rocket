param (
    [Parameter(Mandatory=$true)][string]$t,
    [switch]$c=$false
)

if ( $c )
{
    echo "Performing clean build..."
    rm -R -Force app/$t/build/
}

cmake --preset=$t
pushd app/$t/build/
make
popd