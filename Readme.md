# BitBully: A fast and perfect-playing Connect-4 Agent for Python 3 & C/C++

Generally, a lot of ideas were taken from the great Connect-4 Solver that Pascal Pons developed:

- [http://blog.gamesolver.org/](http://blog.gamesolver.org/)
- [https://github.com/PascalPons/connect4](https://github.com/PascalPons/connect4)

The board logic was inspired by:

- https://tromp.github.io/c4/Connect4.java

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
pip install -e .[dev,ci]
```

```bash
pre-commit install --hook-type commit-msg --hook-type pre-push
```

You can run pre-commit before a commit with:

```bash
pre-commit run
```

## Commitizen

### Bump Version

```bash
cz bump --dry-run # first perform a dry run
cz bump
git push origin tag x.x.x
```

### Push commit and tag atomically

```bash
git push --atomic origin master v0.0.14
```

### Commit types

| Commit Type | Title                    | Description                                                                                                 | Emoji |
|-------------|--------------------------|-------------------------------------------------------------------------------------------------------------|:-----:|
| `feat`      | Features                 | A new feature                                                                                               |   ✨   |
| `fix`       | Bug Fixes                | A bug Fix                                                                                                   |  🐛   |
| `docs`      | Documentation            | Documentation only changes                                                                                  |  📚   |
| `style`     | Styles                   | Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)      |  💎   |
| `refactor`  | Code Refactoring         | A code change that neither fixes a bug nor adds a feature                                                   |  📦   |
| `perf`      | Performance Improvements | A code change that improves performance                                                                     |  🚀   |
| `test`      | Tests                    | Adding missing tests or correcting existing tests                                                           |  🚨   |
| `build`     | Builds                   | Changes that affect the build system or external dependencies (example scopes: gulp, broccoli, npm)         |  🛠   |
| `ci`        | Continuous Integrations  | Changes to our CI configuration files and scripts (example scopes: Travis, Circle, BrowserStack, SauceLabs) |  ⚙️   |
| `chore`     | Chores                   | Other changes that don't modify src or test files                                                           |  ♻️   |
| `revert`    | Reverts                  | Reverts a previous commit                                                                                   |  🗑   |

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

# Git Tags

Delete the tag locally:

```bash
git tag -d <tag_name>
```

Delete the tag on the remote:

```bash
git push origin --delete <tag_name>
```

# Cheatsheet: Most Useful Commands

## **Version Control (Git)**

1. `git status` – Check repository status.
2. `git add -u` – Stage all changes.
3. `git add <files>` – Stage specific files.
4. `git commit -m "<message>"` – Commit changes with a message.
5. `git commit --amend` – Modify the last commit.
6. `git push` – Push changes to the remote repository.
7. `git tag -l` – List tags.
8. `git tag -d <tag>` – Delete a local tag.
9. `git push origin --delete <tag>` – Delete a remote tag.
10. `git diff` – Show differences between changes.
11. `git fetch` – Fetch changes from the remote repository.
12. `git rebase <branch>` – Rebase the current branch.
13. `git merge <branch>` – Merge a branch into the current branch.

## **Jekyll**

1. `bundle exec jekyll serve --incremental` – Run a Jekyll server incrementally.
2. `bundle exec jekyll serve --lsi` – Run Jekyll with LSI (Latent Semantic Indexing).
3. `bundle install` – Install dependencies from the Gemfile.

## **Python Environment Management**

1. `python3 --version` – Check Python version.
2. `python3 -m venv <name>` – Create a virtual environment.
3. `source <venv>/bin/activate` – Activate a virtual environment.
4. `deactivate` – Deactivate the virtual environment.

## **Dependency and Package Management**

1. `pip install <package>` – Install a Python package.
2. `pip install -e .` – Install the package in editable mode.
3. `pip uninstall <package>` – Uninstall a Python package.
4. `pip freeze` – List installed packages.
5. `gem install <gem>` – Install a Ruby gem.
6. `bundle install` – Install Ruby gem dependencies.

## **Building and Publishing Python Packages**

1. `python -m build` – Build a Python package.
2. `twine upload dist/*` – Upload the package to PyPI.

## **Testing and CI/CD**

1. `pre-commit install` – Install pre-commit hooks.
2. `pre-commit run` – Run all pre-commit hooks.
3. `ctest` – Run CMake-based tests.
4. `cibuildwheel --output-dir dist` – Build Python wheels.

## **Commitizen (cz)**

1. `cz init` – Initialize Commitizen configuration.
2. `cz bump` – Automate version bump.
3. `cz bump --dry-run` – Simulate version bump.

## **Node.js and NPM**

1. `nvm install <version>` – Install a specific Node.js version.
2. `node -v` – Check Node.js version.
3. `npm -v` – Check NPM version.
4. `npm install --save-dev <package>` – Install a development dependency.
5. `npx prettier . --check` – Check code formatting with Prettier.
6. `npx prettier . --write` – Format code with Prettier.

## **Docker**

1. `docker compose pull` – Pull updated images for the services.
2. `docker compose up` – Start Docker Compose services.
3. `docker compose up --help` – Display help for Docker Compose.

## **Ruby and Rbenv**

1. `rbenv install <version>` – Install a specific Ruby version.
2. `rbenv local <version>` – Set a local Ruby version for a project.
3. `gem install bundle` – Install the Bundler gem.
4. `bundle install` – Install Ruby project dependencies.

## **Miscellaneous**

1. `ls` – List directory contents.
2. `cd <path>` – Change directory.
3. `history | grep <keyword>` – Search command history.
4. `htop` – Display interactive process viewer.
5. `tmux` – Start a new tmux session.

## **Configuration and Logs**

1. `nano ~/.bashrc` – Edit the bash configuration file.
2. `cat <file>` – View file contents.
3. `pip show <package>` – Display package details.
4. `sudo apt install <package>` – Install a package using APT.

---

These commands now cover additional workflows like managing Node.js and NPM, using Docker, handling Ruby and rbenv, and
working with Jekyll. Let me know if you'd like further categorization or details.
