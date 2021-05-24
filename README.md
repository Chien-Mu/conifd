

# conifd

It is a package of OpenWRT, used to check the connectivity of the network interface.

***

# Config options - section interface

| Name          | Type          | Required      | default       | Description |
| ------------- | ------------- | ------------- | ------------- | ----------- |
| src           | Interface     | no            | (none)        | Interface checked before ping
| dest          | IP address    | yes           | 8.8.8.8       | IP Address or hostname of ping destination
| interval      | seconds       | yes           | 10            | Ping will be sent every 'interval' seconds
| timeout       | seconds       | no            | 3             | After no Ping replies have been received for 'timeout' seconds, the offline scripts will be executed
| before_check  | y or n        | no            | y             | Execute 'CONIFD_CHECK.sh' before executing ping

#### Here is an example config
```
config interface
    option src br-lan
    option dest 8.8.8.8
    option interval 5
    option timeout 3
    option before_check y
```

***

# Shell Scripts

### online and offline

When a interface status changes, scripts in /etc/pingcheck/online.d/ or /etc/pingcheck/offline.d/ 
are called and provided with INTERFACE environment variables, similar to hotplug scripts. 

| Variable      | Description                                                                           |
|---------------|---------------------------------------------------------------------------------------|
| INTERFACE     | linux network interface (e.g. br-lan) which goes online or offline                    |


### CONIFD_CHECK.sh

If before_check is set 'y', the system will call the CONIFD_CHECK.sh script before executing the ping.

| Variable        | Description                                                                           |
|-----------------|---------------------------------------------------------------------------------------|
| INTERFACE       | linux network interface (e.g. br-lan) which goes online or offline                    |

| Return Variable | Description                                                                                    |
|-----------------|------------------------------------------------------------------------------------------------|
| FAIL            | Will not perform ping operation (will not call the scripts in online.d and offline.d)          |
| SUCCESS         | Will execute the action of ping and trigger the script in the online and offline folders       |
| RESTART         | /etc/init.d/conifd restart                                                                     |
