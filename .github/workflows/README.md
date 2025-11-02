# GitHub Actions Workflows

This directory contains GitHub Actions workflows for continuous integration.

## CI Workflow (`ci.yml`)

The CI workflow runs on every push and pull request to validate the code.

### Jobs

1. **Build Firmware** - Builds all firmware environments:
   - `xiao_esp32c6` - Default build
   - `dev` - Development build (with debug flags)
   - `release` - Release build (optimized)
   - `simple` - Simple build (no OTA support)

2. **Compile Tests** - Compiles the test suite to verify:
   - Test code syntax is correct
   - All dependencies resolve
   - Tests can be built successfully

3. **Code Validation** - Validates:
   - Project structure (required files exist)
   - Partition tables are valid
   - Configuration files are present

### Artifacts

Build artifacts (firmware binaries) are uploaded for each environment and kept for 7 days.

### Requirements

- Python 3.11 (for PyYAML support)
- PlatformIO Core
- All build dependencies are automatically installed

### Status Badge

Add this to your README.md to show CI status:

```markdown
![CI](https://github.com/YOUR_USERNAME/water-flow-zigbee/workflows/CI/badge.svg)
```

Replace `YOUR_USERNAME` with your GitHub username.

