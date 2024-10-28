
BUILD_DIR=build
BUNDLE_DIR=$(BUILD_DIR)/bundle
SRC_DIR=src

TEST_SRC_DIR=test
TEST_BIN_DIR=$(BUILD_DIR)/test

M4=m4
M4FLAGS=--include=$(SRC_DIR) -P

CXX=g++
CXXFLAGS=-fPIC -std=c++20 -I$(BUNDLE_DIR)

TARGET=$(BUNDLE_DIR)/xtual.hxx
SOURCE=$(SRC_DIR)/xtual.hxx.m4
COMPONENTS=$(addprefix $(SRC_DIR)/, common.hxx utf32.hxx utf16.hxx utf8.hxx license.hxx)

TESTS=$(addprefix $(TEST_BIN_DIR)/, test-common test-utf32 test-utf16 test-utf8)

.PHONY: all
all: $(TARGET)

.PHONY: test
test: $(TESTS)

.PHONY: clean
clean:
	-@rm -rf $(BUILD_DIR)

$(TARGET): $(SOURCE) $(COMPONENTS)
	-@mkdir -p $(@D)
	$(M4) $(M4FLAGS) $< > $@

$(TEST_BIN_DIR)/%: $(TEST_SRC_DIR)/%.cxx $(TARGET)
	-@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $<
