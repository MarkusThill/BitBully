# Compiler

The code was compiled with:
gcc 13.1.0

# Development (Debian-based Systems)

## Install Python 3

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

## Commitizen

## Bump Version

```bash
cz bump --dry-run # first perform a dry run
cz bump
git push origin tag x.x.x
```

# Publish to PyPI

1. Create a PyPi Test Account

- Log in to Test [PyPI](https://test.pypi.org/account/login/) or create a new account.
- Navigate to "Account Settings" → "API Tokens".
- Generate a new API token if needed.

2. Create a PyPI Account:

- Sign up on [PyPI](https://pypi.org/account/register/).
- Set up 2FA (Two-Factor Authentication) for enhanced security.
- Install Required Tools: Ensure you have build and twine installed for building and publishing:

```bash
pip install build twine
```

3. Update your `~/.pypirc File`:

```yaml
[ distutils ]
  index-servers =
  pypi
  testpypi

  [ testpypi ]
  repository = https://test.pypi.org/legacy/
  username = __token__
  password = <your_api_token>

  [ pypi ]
  repository = https://upload.pypi.org/legacy/
  username = __token__
  password = <your_api_token>
```

## Build the Package

### Check the Readme

```
pip install readme_renderer[md]
python -m readme_renderer README.md
python -m build
```

### Build

```bash
cd /path/to/project
python -m build
```

### Upload to Test PyPI

Upload to Test pypi first:

```bash
twine upload --repository testpypi dist/*
```

Test installation:

```bash
pip install --index-url https://test.pypi.org/simple/ bitbully
```

### Publish to PyPI

```
twine upload dist/*
```

Test installation from PyPI:

```bash
pip install bitbully
```