rm /tmp/semfile
touch /tmp/semfile

rm seqfile
touch seqfile 

echo '0' > seqfile

case "$1" in
    "1" )
        ./RecordLockingSem & ./RecordLockingSem &
        ;;
    "2" )
        ./RecordLockingNone & ./RecordLockingNone &
        ;;
esac
