#!/bin/bash

case "$1" in
    "1" )
        ./AddOneShareMemoryObjectServer
        ./AddOneShareMemoryObjectClient & ./AddOneShareMemoryObjectClient &
        ;;
    "2" )
        ;;
    "3" )
        ;;
esac
