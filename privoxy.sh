#!/bin/bash
    echo "input root password"
    read -s su_password 
    #echo su_password
su - <<EOF
$su_password
    systemctl start privoxy
    echo "start privoxy finish"
EOF
    export http_proxy=http://127.0.0.1:8118
    export https_proxy=http://127.0.0.1:8118
    export ftp_proxy=http://127.0.0.1:8118
    export no_proxy=localhost,10.0.0.0,172.16.0.0,192.168.0.0,192.168.255.0
    #echo "http_proxy:"$http_proxy
    #echo "https_proxy:"$https_proxy
    #echo "ftp_proxy:"$ftp_proxy
    #echo "no_proxy:"$no_proxy
    #echo "set proxy"
    #echo 
    #echo 
    #echo 
    $1
    unset http_proxy https_proxy ftp_proxy no_proxy
    #echo "unset proxy"
su - <<EOF
$su_password
    systemctl stop privoxy
    echo "stop privoxy finish"
EOF
echo


