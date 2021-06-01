
cp /var/log/messages .
grep RSYS_L2 messages > tmpl2.txt
cut -d ":" -f 8- tmpl2.txt > l2.log
rm -rf tmpl2.txt
