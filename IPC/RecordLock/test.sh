rm seqfile
touch seqfile 

echo '0' > seqfile

case "$1" in
    "1" )
        ./RecordLockingFcntl & ./RecordLockingFcntl &
        ;;
    "2" )
        ./RecordLockingNone & ./RecordLockingNone &
        ;;
    "3" )
        ./RecordLockingNone & ./RecordLockingFcntl &
        ;;
esac
