#!/bin/sh

# translate ip to int
function aton()  
{  
    echo $1 | awk '{c=256;split($0,ip,".");print ip[4]+ip[3]*c+ip[2]*c^2+ip[1]*c^3}'  
} 

# translate int to ip
function trans_int_to_ip()
{
    a=$(($1/256/256/256))
    b=$(($1/256/256-$a*256))
    c=$(($1/256-$a*256*256-$b*256))
    d=$(($1-a*256*256*256-$b*256*256-$c*256))
    echo $a"."$b"."$c"."$d
}

lanip=`ifstatus lan | jsonfilter -e "$['ipv4-address'][0]['address']"`
lanmask=`ifstatus lan | jsonfilter -e "$['ipv4-address'][0]['mask']"`

maskbit=`expr 32 - $lanmask`
bit=1
hostnumber=`expr $(( $bit << $maskbit)) - 2`

ipn=$(aton $lanip)

ipn1=$(( $ipn >> $maskbit))
ipn1=$(( $ipn1 << $maskbit))

i=1
while [ $i -le $hostnumber ]
do
    ipn2=`expr $ipn1 + $i`
    ip=$(trans_int_to_ip $ipn2)
	echo $ip
    ping -4 -c 1 -w 1 -I br-lan $ip >/dev/null 2>/dev/null &
    i=`expr $i + 1`
done
echo "-------------------------"
date
ip1='192.168.1'
for ip2 in $(seq 0 254)
do
    ip=${ip1}.${ip2}
	echo $ip
    ping -4 -c 1 -w 1 -I br-lan $ip >/dev/null 2>/dev/null &
done
date
