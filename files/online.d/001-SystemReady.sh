#!/bin/sh
# This file is an example.

if [ $(ubus -S call led status | jsonfilter -e '@.Green') != 'on' ]; then
    ubus -S call led on "{\"color\":\"green\", \"msg\":\"In $0\"}"
fi
