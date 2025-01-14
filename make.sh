#!/bin/bash

# OUT OF DATE NEEDS TO BE UPDATED
echo "Script out of date!"
exit()

# Function to display usage
usage() {
  echo "Usage: $0 -t <target_name> [-c]"
  exit 1
}

# Initialize variables
TARGET=""
CLEAN=0

# Parse command-line arguments
while getopts ":t:c:" opt; do
  case ${opt} in
    ( t )
      TARGET="$OPTARG"
      ;;
    ( \? )
      echo "Invalid option: -$OPTARG" 1>&2
      usage
      ;;
    ( : )
      if [[ "$OPTARG" == "c" ]]; then
        CLEAN=1
      else
        echo "Option -$OPTARG requires an argument." 1>&2
        usage
      fi
      ;;
  esac
done

# Check if the target name is provided
if [ -z "$TARGET" ]; then
  usage
fi

if [ "$CLEAN" == 1 ]; then
    echo "Performing clean build..."
    rm -rf app/$TARGET/build/
fi

cmake --preset=$TARGET
pushd app/$TARGET/build/
cmake --build .
popd