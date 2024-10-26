
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
COMPONENTS=$(addprefix $(SRC_DIR)/,)

TESTS=$(addprefix $(TEST_BIN_DIR)/,)

.PHONY: all
all: $(TARGET) $(TESTS)

.PHONY: clean
clean:
	-@rm -rf $(BUILD_DIR)

$(TARGET): $(SOURCE) $(COMPONENTS)
	-@mkdir -p $(@D)
	$(M4) $(M4FLAGS) $< > $@

$(TEST_BIN_DIR)/%: $(TEST_SRC_DIR)/%.cxx $(TARGET)
	-@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $<
