# Development

## Compiler

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

## Cheatsheet: Most Useful Commands

### **Version Control (Git)**

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

### **Git Submodule Commands**

1. `git submodule status` – Show the status of submodules.
2. `git submodule add <repo-url> <path>` – Add a new submodule.
3. `git submodule update --init --recursive` – Initialize and update submodules.
4. `git submodule add -b <branch> <repo-url> <path>` – Add a submodule tracking a specific branch.

### **Formatting and Linting**

1. `ruff format <file>` – Format Python code using Ruff.
2. `clang-format <file>` – Format C++ code using Clang-Format.

### **CMake Commands**

1. `cmake --version` – Check the version of CMake.
2. `cmake <options>` – Generate build files based on `CMakeLists.txt`.

### **Testing and Debugging**

1. `gdb --version` – Check the version of GDB (GNU Debugger).
2. `gdb <program>` – Debug a program.

### **Python Environment Management**

1. `python3 --version` – Check Python version.
2. `python3 -m venv <name>` – Create a virtual environment.
3. `source <venv>/bin/activate` – Activate a virtual environment.
4. `deactivate` – Deactivate the virtual environment.

### **Dependency and Package Management**

1. `pip install <package>` – Install a Python package.
2. `pip install -e .` – Install the package in editable mode.
3. `pip uninstall <package>` – Uninstall a Python package.
4. `pip freeze` – List installed packages.
5. `gem install <gem>` – Install a Ruby gem.
6. `bundle install` – Install Ruby gem dependencies.
7. `pip install /path/to/<package.whl>` – Install a locally built wheel.
8. `pip uninstall <package>` – Uninstall a Python package.

### **Building and Publishing Python Packages**

1. `python -m build` – Build a Python package.
2. `twine upload dist/*` – Upload the package to PyPI.
3. `python -m build --wheel` – Build only the wheel package.
4. `python -m build --sdist` – Build only the source distribution.
5. `python -m build --platform-tag` – Add a specific platform tag to the build.

### **CIBuildWheel**

1. `cibuildwheel --output-dir <dir>` – Build Python wheels for multiple platforms.
2. `cibuildwheel --output-dir <dir> --verbose` – Build wheels with detailed output.
3. `cibuildwheel --output-dir <dir> --platform <platform>` – Specify a platform for the build (e.g., `linux`, `macos`,
   or `windows`). However, usually, defined in the `pyproject.toml`.

### **Pre-Commit Commands**

1. `pre-commit install --hook-type <type>` – Install specific pre-commit hooks (e.g., `commit-msg`, `pre-push`).
2. `pre-commit migrate-config` – Migrate `.pre-commit-config.yaml` to the latest format.
3. `pre-commit autoupdate` – Update all pre-commit hooks to their latest versions.
4. `pre-commit run --all-files` – Run hooks on all files.

### **Testing and CI/CD**

1. `pre-commit install` – Install pre-commit hooks.
2. `pre-commit run` – Run all pre-commit hooks.
3. `ctest` – Run CMake-based tests.
4. `cibuildwheel --output-dir dist` – Build Python wheels.

### **Commitizen (cz)**

1. `cz init` – Initialize Commitizen configuration.
2. `cz bump` – Automate version bump.
3. `cz bump --dry-run` – Simulate version bump.

### **Doxygen**

1. `sudo apt install doxygen`  – Install Doxygen using APT
2. `apt show doxygen` – displays detailed information about the doxygen package
3. `doxygen -g Doxyfile` – generates a default configuration file named Doxyfile for Doxygen
4. `doxygen Doxyfile` – Generate Doxgen Docs based on configuration in `Doxyfile`
5. `python3 -m http.server 8000 --directory docs/html/` – Serve documentation locally on port `8000`
6. `doxygen -w html header.html footer.html style.css` – Generate template files for customizing Doxygen's output.

Installation script (used in `doxygen.yml`):

```bash
export DOXY_VERSION=1.13.1
echo "Installing doxgen v$DOXY_VERSION"
sudo apt-get update && sudo apt-get install -y graphviz
mkdir doxygen-installer
wget "https://www.doxygen.nl/files/doxygen-${DOXY_VERSION}.linux.bin.tar.gz"
mv "doxygen-${DOXY_VERSION}.linux.bin.tar.gz" doxygen-installer
cd doxygen-installer
gunzip "doxygen-${DOXY_VERSION}.linux.bin.tar.gz"
tar xf "doxygen-${DOXY_VERSION}.linux.bin.tar"
cd "doxygen-${DOXY_VERSION}"
ls -laht
sudo make install
```

### **Configuration and Logs**

1. `nano ~/.bashrc` – Edit the bash configuration file.
2. `cat <file>` – View file contents.
3. `pip show <package>` – Display package details.
4. `sudo apt install <package>` – Install a package using APT.

### **Node.js and NPM**

1. `nvm install <version>` – Install a specific Node.js version.
2. `node -v` – Check Node.js version.
3. `npm -v` – Check NPM version.
4. `npm install --save-dev <package>` – Install a development dependency.
5. `npx prettier . --check` – Check code formatting with Prettier.
6. `npx prettier . --write` – Format code with Prettier.

### **Docker**

1. `docker compose pull` – Pull updated images for the services.
2. `docker compose up` – Start Docker Compose services.
3. `docker compose up --help` – Display help for Docker Compose.

### **Ruby and Rbenv**

1. `rbenv install <version>` – Install a specific Ruby version.
2. `rbenv local <version>` – Set a local Ruby version for a project.
3. `gem install bundle` – Install the Bundler gem.
4. `bundle install` – Install Ruby project dependencies.

### **Jekyll**

1. `bundle exec jekyll serve --incremental` – Run a Jekyll server incrementally.
2. `bundle exec jekyll serve --lsi` – Run Jekyll with LSI (Latent Semantic Indexing).
3. `bundle install` – Install dependencies from the Gemfile.

### **Miscellaneous**

1. `ls` – List directory contents.
2. `cd <path>` – Change directory.
3. `history | grep <keyword>` – Search command history.
4. `htop` – Display interactive process viewer.
5. `tmux` – Start a new tmux session.

---

These commands now cover additional workflows like managing Node.js and NPM, using Docker, handling Ruby and rbenv, and
working with Jekyll. Let me know if you'd like further categorization or details.

# GitHub Snake

<h1 align="center">
<img src="https://raw.githubusercontent.com/MarkusThill/snk/refs/heads/manual-run-output/only-svg/github-contribution-grid-snake.svg" alt="https://raw.githubusercontent.com/MarkusThill/snk/refs/heads/manual-run-output/only-svg/github-contribution-grid-snake.svg" width="90%" >
</h1><br>
