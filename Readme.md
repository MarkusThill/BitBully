# Compiler

The code was compiled with:
gcc 13.1.0

# Development (Debian-based Systems)

## Commitizen

```bash
sudo apt install python3.11
sudo apt install python3.11-venv
```

Configure alternatives, if you have other versions of python installed:

```bash
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.10 1
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.11 2
sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10 1
sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.11 2

sudo update-alternatives --config python

sudo update-alternatives --config python3
```

## Install & Activate virtualenv

```bash
python3 -m venv venv
source venv/bin/activate
```

## Install Dependencies

```bash
pip install commitizen
```

## Bump Version

```bash
git push origin tag x.x.x
```