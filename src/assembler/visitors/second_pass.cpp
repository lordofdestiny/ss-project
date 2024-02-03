//
// Created by djumi on 1/31/2024.
//

#include <exception>
#include <variant>
#include "ast/stmt_t.h"
#include "visitors/second_pass.h"
#include "common/instruction_t.h"
#include "common/util.h"

namespace m_asm::visitor {
    using common::instruction_t;

    void second_pass::visit_global(stmt_t::global_t &global) {
        auto symtab = asm_ref.get().get_symbol_table();
        for (const auto &symbol_name: global.symbols) {
            if (const auto symbol = symtab.find(symbol_name); symbol == nullptr) {
                throw std::logic_error("symbol " + symbol_name + " not found");
            } else {
                symbol->local = 'g';
            }
        }
    }

    void second_pass::visit_section(stmt_t::section_t &section) {
        asm_ref.get().set_current_section(section.name);
    }

    void second_pass::visit_word(stmt_t::word_t &word) {
        for (const auto &value: word.word_values) {
            if (const auto value_ptr = std::get_if<uint32_t>(&value)) {
                asm_ref.get().write_word(*value_ptr);
                continue;
            }

            // Else it holds a string - symbol name
            const auto &symbol_name = std::get<std::string>(value);
            const auto &symtab = asm_ref.get().get_symbol_table();
            const auto symbol_it = std::find_if(
                symtab.begin(), symtab.end(), [&](const auto &symbol) {
                    return symbol.name == symbol_name;
                });
            if (symbol_it == symtab.end()) {
                throw std::logic_error("symbol '" + symbol_name + "' does not exit");
            }
            asm_ref.get().add_relocation(
                asm_ref.get().current_section().data.size() + 4,
                symbol_it->local
                    ? symbol_it->section_index
                    : std::distance(symtab.begin(), symbol_it),
                symbol_it->local ? symbol_it->value : 0
            );

            asm_ref.get().write_word(0);
        }
    }

    void second_pass::visit_skip(stmt_t::skip_t &skip) {
        asm_ref.get().write_zeros(skip.size);
    }

    void second_pass::visit_ascii(stmt_t::ascii_t &ascii) {
        asm_ref.get().write_string(ascii.value);
    }

    void second_pass::visit_equals([[maybe_unused]] stmt_t::equals_t &equals) {
    }

    void second_pass::visit_paramless_instr(stmt_t::instr_t::paramless_t &paramless) {
        switch (paramless.type) {
        case mnemonic_t::HALT: {
            asm_ref.get().write_word(instruction_t::make_halt().to_word());
            break;
        }
        case mnemonic_t::INT: {
            asm_ref.get().write_word(instruction_t::make_int().to_word());
            break;
        }
        case mnemonic_t::IRET: {
            asm_ref.get().write_word(instruction_t::make_load(
                instruction_t::load_mode::REG_DISP_IND_POSTINC,
                reg_t::pc, reg_t::sp, 0, 4
            ).to_word());
            asm_ref.get().write_word(instruction_t::make_load(
                instruction_t::load_mode::WRITE_CSR_REG_DISP_IND_POSTINC,
                0, reg_t::sp, 0, 4
            ).to_word());
            break;
        }
        case mnemonic_t::RET: {
            asm_ref.get().write_word(instruction_t::make_load(
                instruction_t::load_mode::REG_DISP_IND,
                reg_t::pc, reg_t::sp, 0, 4
            ).to_word());
            break;
        }
        default:
            break;
        }
    }

    void second_pass::visit_unary_instr(stmt_t::instr_t::unary_t &unary) {
        switch (unary.type) {
        case mnemonic_t::PUSH: {
            asm_ref.get().write_word(instruction_t::make_store(
                instruction_t::store_mode::REG_IND_PREINC,
                reg_t::sp, 0, unary.reg, -4
            ).to_word());
            break;
        }
        case mnemonic_t::POP: {
            asm_ref.get().write_word(instruction_t::make_load(
                instruction_t::load_mode::REG_DISP_IND_POSTINC,
                unary.reg, reg_t::sp, 0, 4
            ).to_word());
            break;
        }
        case mnemonic_t::NOT: {
            asm_ref.get().write_word(instruction_t::make_logic(
                instruction_t::logic_mode::NOT,
                unary.reg, unary.reg, 0
            ).to_word());
            break;
        }
        default:
            break;
        }
    }

    void second_pass::visit_binary_instr(stmt_t::instr_t::binary_t &binary) {
        switch (binary.type) {
        case mnemonic_t::ADD: {
            asm_ref.get().write_word(instruction_t::make_arithmetic(
                instruction_t::arithmetic_mode::ADD,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::SUB: {
            asm_ref.get().write_word(instruction_t::make_arithmetic(
                instruction_t::arithmetic_mode::SUB,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::MUL: {
            asm_ref.get().write_word(instruction_t::make_arithmetic(
                instruction_t::arithmetic_mode::MUL,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::DIV: {
            asm_ref.get().write_word(instruction_t::make_arithmetic(
                instruction_t::arithmetic_mode::DIV,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::AND: {
            asm_ref.get().write_word(instruction_t::make_logic(
                instruction_t::logic_mode::AND,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::OR: {
            asm_ref.get().write_word(instruction_t::make_logic(
                instruction_t::logic_mode::OR,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::XOR: {
            asm_ref.get().write_word(instruction_t::make_logic(
                instruction_t::logic_mode::XOR,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::SHL: {
            asm_ref.get().write_word(instruction_t::make_shift(
                instruction_t::shift_mode::LEFT,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::SHR: {
            asm_ref.get().write_word(instruction_t::make_shift(
                instruction_t::shift_mode::RIGHT,
                binary.second, binary.second, binary.first
            ).to_word());
            break;
        }
        case mnemonic_t::CSRRD: {
            asm_ref.get().write_word(instruction_t::make_load(
                instruction_t::load_mode::READ_CSR,
                binary.second, binary.first, 0, 0
            ).to_word());
            break;
        }
        case mnemonic_t::CSRWR: {
            asm_ref.get().write_word(instruction_t::make_load(
                instruction_t::load_mode::WRITE_CSR,
                binary.second, binary.first, 0, 0
            ).to_word());
            break;
        }
        default:
            break;
        }
    }

    void second_pass::visit_memory_instr(stmt_t::instr_t::memory_t &memory) {
        if (memory.type == mnemonic_t::LD) {
            switch (memory.operand.type) {
            case operand_t::type_t::LITERAL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, 0, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(memory.operand.value));
                break;
            }
            case operand_t::type_t::SYMBOL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg, reg_t::pc, 0, 4
                ).to_word());
                const auto &symbol_name = std::get<symbol_t>(memory.operand.value);
                const auto &symtab = asm_ref.get().get_symbol_table();
                const auto symbol_it = std::find_if(
                    symtab.begin(), symtab.end(), [&](const auto &symbol) {
                        return symbol.name == symbol_name;
                    });
                if (symbol_it == symtab.end()) {
                    throw std::logic_error("symbol '" + symbol_name + "' does not exit");
                }
                asm_ref.get().add_relocation(
                    asm_ref.get().current_section().data.size() + 4,
                    symbol_it->local
                        ? symbol_it->section_index
                        : std::distance(symtab.begin(), symbol_it),
                    symbol_it->local ? symbol_it->value : 0
                );
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, 0, 0, 4
                ).to_word());
                asm_ref.get().write_word(0);
                break;
            }
            case operand_t::type_t::LITERAL_ADDR: {
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, 0, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(memory.operand.value));
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg, memory.reg, 0, 0
                ).to_word());
                break;
            }
            case operand_t::type_t::SYMBOL_ADDR: {
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg, reg_t::pc, 0, 4
                ).to_word());
                const auto &symbol_name = std::get<symbol_t>(memory.operand.value);
                const auto &symtab = asm_ref.get().get_symbol_table();
                const auto symbol_it = std::find_if(
                    symtab.begin(), symtab.end(), [&](const auto &symbol) {
                        return symbol.name == symbol_name;
                    });
                if (symbol_it == symtab.end()) {
                    throw std::logic_error("symbol '" + symbol_name + "' does not exit");
                }
                asm_ref.get().add_relocation(
                    asm_ref.get().current_section().data.size() + 4,
                    symbol_it->local
                        ? symbol_it->section_index
                        : std::distance(symtab.begin(), symbol_it),
                    symbol_it->local ? symbol_it->value : 0
                );
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, 0, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(memory.operand.value));
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg, memory.reg, 0, 0
                ).to_word());
                break;
            }
            case operand_t::type_t::REG_VALUE: {
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_MOVE_INC,
                    memory.reg,
                    std::get<reg_t>(memory.operand.value),
                    0, 0
                ).to_word());
                break;
            }
            case operand_t::type_t::REG_MEMORY_ADDR: {
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg,
                    std::get<reg_t>(memory.operand.value),
                    0, 0
                ).to_word());
                break;
            }
            case operand_t::type_t::REG_ADDR_LITERAL_OFFSET: {
                const auto &[src_reg, literal] = std::get<reg_literal_t>(memory.operand.value);
                if (!common::util::is_valid_literal(literal)) {
                    throw std::logic_error("Literal can't be written as a 12-bit signed integer");
                }
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg, src_reg, 0, literal
                ).to_word());
                break;
            }
            case operand_t::type_t::REG_ADDR_SYMBOL_OFFSET: {
                const auto &[src_reg, symbol_name] = std::get<reg_symbol_t>(memory.operand.value);
                const auto symbol = asm_ref.get().get_symbol_table().find(symbol_name);
                if (symbol == nullptr || !symbol->has_value) {
                    throw std::logic_error(
                        "Symbol must be defined a compile time and"
                        " have a value for it to be used here.");
                }
                if (!common::util::is_valid_literal(symbol->value)) {
                    throw std::logic_error("Literal can't be written as a 12-bit signed integer");
                }
                asm_ref.get().write_word(instruction_t::make_load(
                    instruction_t::load_mode::REG_DISP_IND,
                    memory.reg, src_reg, 0, symbol->value
                ).to_word());
                break;
            }
            }
        }

        if (memory.type == mnemonic_t::ST) {
            switch (memory.operand.type) {
            case operand_t::type_t::LITERAL_VALUE:
            case operand_t::type_t::SYMBOL_VALUE: {
                throw std::logic_error("Cannot use a store instruction with immidiate addressing");
            }
            case operand_t::type_t::LITERAL_ADDR: {
                asm_ref.get().write_word(instruction_t::make_store(
                    instruction_t::store_mode::REG_DISP_IND,
                    reg_t::pc, 0, memory.reg, 4
                ).to_word());
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, 0, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(memory.operand.value));
                break;
            }
            case operand_t::type_t::SYMBOL_ADDR: {
                asm_ref.get().write_word(instruction_t::make_store(
                    instruction_t::store_mode::REG_DISP_IND,
                    reg_t::pc, 0, memory.reg, 4
                ).to_word());
                const auto &symbol_name = std::get<symbol_t>(memory.operand.value);
                const auto &symtab = asm_ref.get().get_symbol_table();
                const auto symbol_it = std::find_if(
                    symtab.begin(), symtab.end(), [&](const auto &symbol) {
                        return symbol.name == symbol_name;
                    });
                if (symbol_it == symtab.end()) {
                    throw std::logic_error("symbol '" + symbol_name + "' does not exit");
                }
                asm_ref.get().add_relocation(
                    asm_ref.get().current_section().data.size() + 4,
                    symbol_it->local
                        ? symbol_it->section_index
                        : std::distance(symtab.begin(), symbol_it),
                    symbol_it->local ? symbol_it->value : 0
                );
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, 0, 0, 4
                ).to_word());
                asm_ref.get().write_word(0);
                break;
            }
            case operand_t::type_t::REG_VALUE: {
                asm_ref.get().write_word(instruction_t::make_store(
                    instruction_t::store_mode::REG_DISP_IND,
                    std::get<reg_t>(memory.operand.value),
                    0, memory.reg, 0
                ).to_word());
                break;
            }
            case operand_t::type_t::REG_MEMORY_ADDR: {
                asm_ref.get().write_word(instruction_t::make_store(
                    instruction_t::store_mode::MEM_REG_DISP_IND,
                    std::get<reg_t>(memory.operand.value),
                    0, memory.reg, 0
                ).to_word());
                break;
            }
            case operand_t::type_t::REG_ADDR_LITERAL_OFFSET: {
                const auto &[dest_reg, literal] = std::get<reg_literal_t>(memory.operand.value);
                if (!common::util::is_valid_literal(literal)) {
                    throw std::logic_error("Literal can't be written as a 12-bit signed integer");
                }
                asm_ref.get().write_word(instruction_t::make_store(
                    instruction_t::store_mode::MEM_REG_DISP_IND,
                    dest_reg, 0, memory.reg, literal
                ).to_word());
                break;
            }
            case operand_t::type_t::REG_ADDR_SYMBOL_OFFSET: {
                const auto &[dest_reg, symbol_name] = std::get<reg_symbol_t>(memory.operand.value);
                const auto symbol = asm_ref.get().get_symbol_table().find(symbol_name);
                if (symbol == nullptr || !symbol->has_value) {
                    throw std::logic_error(
                        "Symbol must be defined a compile time and"
                        " have a value for it to be used here.");
                }
                if (!common::util::is_valid_literal(symbol->value)) {
                    throw std::logic_error("Literal can't be written as a 12-bit signed integer");
                }
                asm_ref.get().write_word(instruction_t::make_store(
                    instruction_t::store_mode::MEM_REG_DISP_IND,
                    dest_reg, 0, memory.reg, symbol->value
                ).to_word());
                break;
            }
            }
        }
    }

    void second_pass::visit_uncond_instr(stmt_t::instr_t::uncond_t &uncond) {
        if (uncond.type == mnemonic_t::CALL) {
            switch (uncond.operand.type) {
            case operand_t::type_t::LITERAL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_call(
                    instruction_t::call_mode::MEM_IND_DISP,
                    reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(uncond.operand.value));
                break;
            }
            case operand_t::type_t::SYMBOL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_call(
                    instruction_t::call_mode::MEM_IND_DISP,
                    reg_t::pc, 0, 4
                ).to_word());
                const auto &symbol_name = std::get<symbol_t>(uncond.operand.value);
                const auto &symtab = asm_ref.get().get_symbol_table();
                const auto symbol_it = std::find_if(
                    symtab.begin(), symtab.end(), [&](const auto &symbol) {
                        return symbol.name == symbol_name;
                    });
                if (symbol_it == symtab.end()) {
                    throw std::logic_error("symbol '" + symbol_name + "' does not exit");
                }
                asm_ref.get().add_relocation(
                    asm_ref.get().current_section().data.size() + 4,
                    symbol_it->local
                        ? symbol_it->section_index
                        : std::distance(symtab.begin(), symbol_it),
                    symbol_it->local ? symbol_it->value : 0
                );
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(0);
                break;
            }
            default:
                throw std::logic_error("Invalid operand type");
            }
        }
        if (uncond.type == mnemonic_t::JMP) {
            switch (uncond.operand.type) {
            case operand_t::type_t::LITERAL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, 0, 0, 0
                ).to_word());
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(uncond.operand.value));
                break;
            }
            case operand_t::type_t::SYMBOL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, 0, 0, 0
                ).to_word());
                const auto &symbol_name = std::get<symbol_t>(uncond.operand.value);
                const auto &symtab = asm_ref.get().get_symbol_table();
                const auto symbol_it = std::find_if(
                    symtab.begin(), symtab.end(), [&](const auto &symbol) {
                        return symbol.name == symbol_name;
                    });
                if (symbol_it == symtab.end()) {
                    throw std::logic_error("symbol '" + symbol_name + "' does not exit");
                }
                asm_ref.get().add_relocation(
                    asm_ref.get().current_section().data.size() + 4,
                    symbol_it->local
                        ? symbol_it->section_index
                        : std::distance(symtab.begin(), symbol_it),
                    symbol_it->local ? symbol_it->value : 0
                );
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(0);
                break;
            }
            default:
                throw std::logic_error("Invalid operand type");
            }
        }
    }

    void second_pass::visit_branch_instr(stmt_t::instr_t::branch_t &branch) {
        if (branch.type == mnemonic_t::BEQ) {
            switch (branch.operand.type) {
            case operand_t::type_t::LITERAL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::EQ_REG_IND_DISP,
                    reg_t::pc, branch.reg1, branch.reg2, 4
                ).to_word());
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(branch.operand.value));
                break;
            }
            case operand_t::type_t::SYMBOL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::EQ_REG_IND_DISP,
                    reg_t::pc, branch.reg1, branch.reg2, 4
                ).to_word());
                const auto &symbol_name = std::get<symbol_t>(branch.operand.value);
                const auto &symtab = asm_ref.get().get_symbol_table();
                const auto symbol_it = std::find_if(
                    symtab.begin(), symtab.end(), [&](const auto &symbol) {
                        return symbol.name == symbol_name;
                    });
                if (symbol_it == symtab.end()) {
                    throw std::logic_error("symbol '" + symbol_name + "' does not exit");
                }
                asm_ref.get().add_relocation(
                    asm_ref.get().current_section().data.size() + 4,
                    symbol_it->local
                        ? symbol_it->section_index
                        : std::distance(symtab.begin(), symbol_it),
                    symbol_it->local ? symbol_it->value : 0
                );
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(0);
                break;
            }
            default:
                break;
            }
        }

        if (branch.type == mnemonic_t::BNE) {
            switch (branch.operand.type) {
            case operand_t::type_t::LITERAL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::NE_REG_IND_DISP,
                    reg_t::pc, branch.reg1, branch.reg2, 4
                ).to_word());
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(branch.operand.value));
                break;
            }
            case operand_t::type_t::SYMBOL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::NE_REG_IND_DISP,
                    reg_t::pc, branch.reg1, branch.reg2, 4
                ).to_word());
                const auto &symbol_name = std::get<symbol_t>(branch.operand.value);
                const auto &symtab = asm_ref.get().get_symbol_table();
                const auto symbol_it = std::find_if(
                    symtab.begin(), symtab.end(), [&](const auto &symbol) {
                        return symbol.name == symbol_name;
                    });
                if (symbol_it == symtab.end()) {
                    throw std::logic_error("symbol '" + symbol_name + "' does not exit");
                }
                asm_ref.get().add_relocation(
                    asm_ref.get().current_section().data.size() + 4,
                    symbol_it->local
                        ? symbol_it->section_index
                        : std::distance(symtab.begin(), symbol_it),
                    symbol_it->local ? symbol_it->value : 0
                );
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(0);
                break;
            }
            default:
                break;
            }
        }

        if (branch.type == mnemonic_t::BGT) {
            switch (branch.operand.type) {
            case operand_t::type_t::LITERAL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::GT_REG_IND_DISP,
                    reg_t::pc, branch.reg1, branch.reg2, 4
                ).to_word());
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(std::get<number_t>(branch.operand.value));
                break;
            }
            case operand_t::type_t::SYMBOL_VALUE: {
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::GT_REG_IND_DISP,
                    reg_t::pc, branch.reg1, branch.reg2, 4
                ).to_word());
                const auto &symbol_name = std::get<symbol_t>(branch.operand.value);
                const auto &symtab = asm_ref.get().get_symbol_table();
                const auto symbol_it = std::find_if(
                    symtab.begin(), symtab.end(), [&](const auto &symbol) {
                        return symbol.name == symbol_name;
                    });
                if (symbol_it == symtab.end()) {
                    throw std::logic_error("symbol '" + symbol_name + "' does not exit");
                }
                asm_ref.get().add_relocation(
                    asm_ref.get().current_section().data.size() + 4,
                    symbol_it->local
                        ? symbol_it->section_index
                        : std::distance(symtab.begin(), symbol_it),
                    symbol_it->local ? symbol_it->value : 0
                );
                asm_ref.get().write_word(instruction_t::make_jump(
                    instruction_t::jump_mode::REG_IND_DISP,
                    reg_t::pc, reg_t::pc, 0, 4
                ).to_word());
                asm_ref.get().write_word(0);
                break;
            }
            default:
                break;
            }
        }
    }
} // m_asm::visitors
