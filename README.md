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

<img width="945" height="747" alt="image" src="https://github.com/user-attachments/assets/264b358e-5e7e-439b-a57a-494bee64bc87" />

# ⚔️ ft_sword (Bonus)

ft_sword is a bonus, it opens a socket on your computer to wich ft_shield will connect telling you it was launched and on wich ip (Make sure to edit SWORD_IP and SWORD_PORT to your computer's in ctx.h)
It also receives all logs from ft_shield and files sent via the transfer command.
To use it, just:

```
make bonus
```

```
./ft_sword
```

### Example

<img width="448" height="80" alt="image" src="https://github.com/user-attachments/assets/e678f63b-d994-4664-bfd9-82e6c45df97f" />

## Notes

DO NOT USE THIS PROJECT FOR MALICIOUS INTENTS, it aint my fault if you do bad things with it.
