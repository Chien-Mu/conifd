#!/bin/sh

###### ENV ######
# INTERFACE
# FAIL          // Will not perform ping operation (will not call the scripts in online.d and offline.d).
# SUCCESS       // Will execute the action of ping and trigger the script in the online and offline folders.
# RESTART       // /etc/init.d/conifd restart


# ====== example ======
if [ -z "$INTERFACE" ]; then
    ubus -S call led blink "{\"color\":\"red\", \"msg\":\"In $0: conifd uci error (option interface=$INTERFACE) \"}"
    exit $FAIL
fi

if [ -f /tmp/wps_son_active ]; then
    exit $FAIL
fi

re_done="$(uci get network.globals.re_done)"
current_ip=$(ifconfig "$INTERFACE" | grep 'inet addr:' | cut -d ':' -f 2 | cut -d ' ' -f 1)
default_gateway=$(ip route | awk '/default/ { print $3 }')
if [ "$re_done" = '1' ]; then
    if [ -z "$current_ip" ]; then
        if [ $(ubus -S call led status | jsonfilter -e '@.Green') != 'blink' ]; then
            ubus -S call led blink "{\"color\":\"green\", \"msg\":\"In $0: $INTERFACE does not exist ip address.\"}"]
        fi
        exit $FAIL
    elif [ -z "$default_gateway" ]; then
        if [ $(ubus -S call led status | jsonfilter -e '@.Green') != 'blink' ]; then
            ubus -S call led blink "{\"color\":\"green\", \"msg\":\"In $0: does not exist default gateway($default_gateway).\"}"
        fi
        exit $FAIL
    elif [ "$current_ip" -a "$default_gateway" ]; then
        if [ "$default_gateway" != $(uci get conifd.@interface[0].dest) ]; then
            uci set conifd.@interface[0].dest="$default_gateway"
            uci commit conifd
            exit $RESTART
        fi
    fi
else
    exit $FAIL
fi
# ====== example end ======

exit $SUCCESS
