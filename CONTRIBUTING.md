# Contributing to Water Flow Meter

Thank you for your interest in contributing to this project! This document provides guidelines and instructions for contributing.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How to Contribute](#how-to-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Pull Request Process](#pull-request-process)
- [Reporting Issues](#reporting-issues)

## 📜 Code of Conduct

This project follows the [Contributor Covenant Code of Conduct](https://www.contributor-covenant.org/). By participating, you are expected to uphold this code.

### Our Standards

- Be respectful and inclusive
- Welcome diverse perspectives
- Focus on constructive feedback
- Be patient and helpful

## 🤝 How to Contribute

### Reporting Bugs

If you find a bug:

1. **Check Existing Issues:**
   - Search GitHub issues
   - Check if bug already reported
   - Review similar issues

2. **Create New Issue:**
   - Use bug report template
   - Provide detailed description
   - Include steps to reproduce
   - Add relevant logs/config

3. **Include Information:**
   - Hardware version
   - Firmware version
   - Software versions
   - Error messages/logs

### Suggesting Enhancements

If you have an enhancement idea:

1. **Check Existing Issues:**
   - Search for similar requests
   - Review roadmap
   - Check planned features

2. **Create Enhancement Issue:**
   - Use feature request template
   - Describe enhancement clearly
   - Explain use case
   - Consider implementation

### Contributing Code

1. **Fork Repository:**
   - Fork on GitHub
   - Clone locally
   - Create feature branch

2. **Make Changes:**
   - Follow coding standards
   - Write tests if applicable
   - Update documentation

3. **Submit Pull Request:**
   - Create pull request
   - Describe changes
   - Reference related issues

## 🔧 Development Setup

### Prerequisites

- [ ] Arduino IDE 2.x or 1.8.x
- [ ] ESP32 board support (v3.0.0+)
- [ ] XIAO ESP32C6 hardware (or compatible)
- [ ] Git for version control

### Setup Steps

1. **Clone Repository:**
   ```bash
   git clone https://github.com/yourusername/water-flow-zigbee.git
   cd water-flow-zigbee
   ```

2. **Install Dependencies:**
   - Install Arduino IDE
   - Install ESP32 board package
   - Install required libraries

3. **Configure Arduino IDE:**
   - Add ESP32 board manager URL
   - Select XIAO ESP32C6 board
   - Configure serial port

4. **Open Project:**
   - Open `src/water_flow_meter.ino`
   - Verify compilation
   - Test upload

### Project Structure

```
water-flow-zigbee/
├── src/                    # Source code
│   └── water_flow_meter.ino
├── examples/               # Example sketches
├── docs/                   # Documentation
├── tests/                  # Test procedures
└── config/                 # Configuration examples
```

## 📝 Coding Standards

### Code Style

- **Indentation:** 2 spaces (no tabs)
- **Line Length:** 80 characters max
- **Naming:**
  - Functions: `camelCase`
  - Variables: `camelCase`
  - Constants: `UPPER_CASE`
  - Classes: `PascalCase`

### Code Formatting

```cpp
// Good: Clear structure, proper spacing
void calculateFlow() {
    static unsigned long lastCheck = 0;
    
    if (millis() - lastCheck >= 1000) {
        // Calculate flow rate
        flowRate = pulseCount / 7.5;
        lastCheck = millis();
    }
}

// Bad: No spacing, unclear structure
void calculateFlow(){static unsigned long lastCheck=0;if(millis()-lastCheck>=1000){flowRate=pulseCount/7.5;lastCheck=millis();}}
```

### Comments

- **Function Comments:**
  ```cpp
  /**
   * Calculate flow rate from pulse count
   * @param pulses Number of pulses counted
   * @return Flow rate in L/min
   */
  float calculateFlowRate(uint32_t pulses) {
      // Implementation
  }
  ```

- **Inline Comments:**
  ```cpp
  // Calculate flow rate every second
  if (millis() - lastCheck >= 1000) {
      // Update flow rate
      flowRate = pulseCount / 7.5;
  }
  ```

### Documentation

- Update README if needed
- Add code comments
- Update step guides if applicable
- Document configuration changes

## 🔍 Testing

### Before Submitting

1. **Test Changes:**
   - Verify code compiles
   - Test on hardware if possible
   - Verify no regressions

2. **Test Checklist:**
   - [ ] Code compiles without errors
   - [ ] No warnings (or justified warnings)
   - [ ] Functionality works as expected
   - [ ] Documentation updated

### Test Procedures

- Follow testing guide: [docs/TESTING.md](docs/TESTING.md)
- Test affected features
- Verify integration works
- Check Home Assistant integration (if applicable)

## 📤 Pull Request Process

### Before Creating PR

1. **Sync with Upstream:**
   ```bash
   git fetch upstream
   git checkout main
   git merge upstream/main
   ```

2. **Create Feature Branch:**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make Changes:**
   - Write code
   - Test thoroughly
   - Update documentation

4. **Commit Changes:**
   ```bash
   git add .
   git commit -m "Add feature: brief description"
   ```

5. **Push to Fork:**
   ```bash
   git push origin feature/your-feature-name
   ```

### Creating Pull Request

1. **Create PR on GitHub:**
   - Click "New Pull Request"
   - Select your fork and branch
   - Fill out PR template

2. **PR Description:**
   - Describe changes clearly
   - Reference related issues
   - List testing performed
   - Include screenshots (if applicable)

3. **PR Checklist:**
   - [ ] Code follows standards
   - [ ] Tests pass
   - [ ] Documentation updated
   - [ ] No breaking changes (or documented)

### Review Process

1. **Automated Checks:**
   - Code compiles
   - Tests pass
   - Code style checks

2. **Manual Review:**
   - Code review by maintainers
   - Functionality verification
   - Documentation review

3. **Feedback:**
   - Address review comments
   - Make requested changes
   - Resubmit for review

## 🐛 Reporting Issues

### Bug Report Template

```markdown
## Bug Description
Brief description of the bug

## Steps to Reproduce
1. Step one
2. Step two
3. ...

## Expected Behavior
What should happen

## Actual Behavior
What actually happens

## Environment
- Hardware: XIAO ESP32C6
- Firmware Version: x.x.x
- Arduino IDE Version: x.x.x
- ESP32 Board Package: x.x.x

## Logs
```
Paste relevant logs here
```

## Additional Information
Any other relevant information
```

### Feature Request Template

```markdown
## Feature Description
Brief description of the feature

## Use Case
Why is this feature needed?

## Proposed Solution
How should it work?

## Alternatives Considered
Other approaches considered

## Additional Information
Any other relevant information
```

## 📚 Documentation

### Contributing to Documentation

Documentation improvements welcome:

1. **Clarity:**
   - Fix unclear instructions
   - Add missing steps
   - Improve explanations

2. **Completeness:**
   - Add missing information
   - Complete examples
   - Add troubleshooting tips

3. **Accuracy:**
   - Update outdated information
   - Fix incorrect information
   - Verify all steps work

### Documentation Structure

- [README.md](README.md) - Main project documentation
- [docs/](docs/) - Step-by-step guides
- Code comments - Function documentation

## ✅ Checklist for Contributors

Before submitting:

- [ ] Code follows style guidelines
- [ ] Code compiles without errors
- [ ] Code tested on hardware
- [ ] Documentation updated
- [ ] Tests pass (if applicable)
- [ ] No breaking changes (or documented)
- [ ] Commit messages clear
- [ ] PR description complete

## 🙏 Thank You!

Thank you for contributing to this project! Your contributions help make this project better for everyone.

## 📞 Questions?

- Open an issue for questions
- Check existing issues/discussions
- Review documentation

---

**Ready to contribute?** Fork the repository and start coding!

