#!/bin/sh
# This file is an example.

if [ $(ubus -S call led status | jsonfilter -e '@.Green') != 'blink' ]; then
    ubus -S call led blink "{\"color\":\"green\", \"msg\":\"In $0\"}"
fi
