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

transfer:
Sends a file given as argument to ft_sword

help:
Show help

### Example

<img width="953" height="829" alt="Screenshot from 2025-12-05 13-11-55" src="https://github.com/user-attachments/assets/3bf78c52-b628-4d97-a11a-ab4bd146e1b7" />

# ⚔️ ft_sword (Bonus)

ft_sword is a bonus, it opens a socket on your computer to wich ft_shield will connect telling you it was launched and on wich ip
It also receives all logs from ft_shield and files sent via the transfer command.
To use it, just:

```
make bonus
```

```
./ft_sword
```

## Other bonuses

The subject asks for 5 total bonuses of our choice to add to the project:
I chose ft_sword, logs, file transfer, additional commands on the socket and the ability to have it work even if its not started as root.

### Example

<img width="941" height="490" alt="image" src="https://github.com/user-attachments/assets/8a6c5067-0fb8-4e8e-8e1b-771b58ff8795" />

## Notes

DO NOT USE THIS PROJECT WITH MALICIOUS INTENTS, it aint my fault if you do bad things with it.
