# 🛡️ ft_shield

ft_shield is one of the first project in the virus branch of 42Advanced
The basic idea is to launch a trojan as daemon and it needs to restart whenever we reboot the targeted machine.

## Compile/Start

(Beware, the project is a VIRUS)

To compile the project:

```
make
```

To start the project:

```
sudo ./ft_shield
```

The program needs to be launched as sudo as it needs to set itself as a system service and it creates files in /var/.

## Usage

You can connect to ft_shield using nc, for example:

```
nc <IP> 4242
```

You can then login using the password (mbatty42)
You have access to quit, help and shell

shell:
Start a shell on the socket

quit:
Makes ft_shield exit

help:
Show help

## Notes

DO NOT USE THIS PROJECT FOR MALICIOUS INTENTS, it aint my fault if you do bad things with it.
