#ifndef _PKMN_GEN1_HPP_
#define _PKMN_GEN1_HPP_

#include "Constants.hpp"
#include "gen1/Gen1Structs.hpp"
#include "gen1/Gen1Enums.hpp"
#include "IGlobal.hpp"
#include "RomInterface.hpp"
#include "Utils.hpp"

#include <fstream>
#include <memory>

//template<class Derived>
struct Gen1 : IGlobal {
    Gen1(std::unique_ptr<RomInterface>&& r) : m_rom(std::move(r))
    {
	}

    auto get_checksum() const -> std::uint8_t override;
    auto calc_checksum() const -> std::uint8_t override;
	auto set_checksum() -> void;

	auto load_file() -> void;
	auto save_changes(std::filesystem::path const & file_name) const -> bool;

	auto get_player_name() const -> std::string;
	auto set_player_name(std::string const &  name) -> void;

	auto get_pokedex_owned(std::uint8_t const index) const -> bool;
	auto set_pokedex_owned(std::uint8_t const index, bool const owned) -> void;

	auto get_pokedex_seen(std::uint8_t const index) const -> bool;
	auto set_pokedex_seen(std::uint8_t const index, bool const seen) -> void;

	auto get_money() const -> std::uint32_t;
	auto set_money(std::uint32_t const value) -> void;

	auto get_casino_coins() const -> std::uint16_t;
	auto set_casino_coins(std::uint16_t const value) -> void;

	auto get_time_played(struct Gen1Structs::pkmn_time *dest) const -> void;
	auto set_time_played(std::uint16_t const hours, std::uint8_t const minutes, std::uint8_t const seconds) -> void;

	auto get_current_pc_box() const -> std::uint8_t;
	auto set_current_pc_box(std::uint8_t const index) -> void;

	auto get_badge(enum Gen1Enums::badges const badge) const -> std::uint8_t;
	auto set_badge(enum Gen1Enums::badges const badge) -> void;

	auto get_option(enum Gen1Enums::options_flags const flag) const -> std::uint8_t;
	auto set_option(enum Gen1Enums::options_flags const flag) -> void;

	auto get_pikachu_friendship() const -> std::uint8_t;
	auto set_pikachu_friendship(std::uint8_t const value) -> void;

	auto get_item_bag(std::uint8_t const index) const -> struct Gen1Structs::item *;
	auto set_item_bag(struct Gen1Structs::item *items, std::uint8_t const index, std::uint8_t const item, std::uint8_t const count) -> void;

	auto get_item_pc(std::uint8_t const index) const -> struct Gen1Structs::item *;
	auto set_item_pc(struct Gen1Structs::item *items, std::uint8_t const index, std::uint8_t const item, std::uint8_t const count) -> void;

	auto get_character_code(std::uint8_t const c) const -> std::uint8_t;

    private:
        std::unique_ptr<RomInterface> m_rom;

        std::uint8_t* player_name;
        std::uint8_t* pokedex_owned;
        std::uint8_t* pokedex_seen;
        std::uint8_t* money;
        std::uint8_t* rival_name;
        std::uint8_t* options;
        std::uint8_t* badges;
        std::uint8_t* pikachu_friendship;
        std::uint8_t* current_pc_box;
        std::uint8_t* casino_coins;
        std::uint8_t* item_count;
        std::uint8_t* current_box_list;
        std::uint8_t* checksum;

        std::uint16_t* player_trainer_id;

        struct Gen1Structs::items_bag* pocket_item_list;
        struct Gen1Structs::items_pc* pc_item_list;
        struct Gen1Structs::pkmn_time* time_played;
        struct Gen1Structs::pkmn_data_struct* team_pokemon_list;
        struct Gen1Structs::pkmn_box* pc_box[12];
};

#endif
