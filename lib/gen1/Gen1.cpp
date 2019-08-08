#include "Gen1.hpp"

auto Gen1::get_checksum() const -> std::uint8_t
{
    return this->m_rom->data[C::GEN1::OFFSETS::CHECKSUM];
}

auto Gen1::calc_checksum() const -> std::uint8_t
{
    std::uint8_t checksum{ 0 };
    for (auto i = C::GEN1::OFFSETS::CHECKSUM_INIT_OFFSET;
         i <= C::GEN1::OFFSETS::CHECKSUM_END_OFFSET; i++) {
        checksum += m_rom->data[i];
    }
    return ~checksum;
}

auto Gen1::set_checksum() -> void
{
	auto checksum = this->calc_checksum();
	// NOTE: *(this->checksum) = checksum
	this->checksum[0] = checksum;
}

auto Gen1::load_file() -> void
{
	if (this->m_rom == nullptr) {
		return;
	}

	this->player_name = &this->m_rom->data[C::GEN1::OFFSETS::PLAYER_NAME];
	this->pokedex_owned = &this->m_rom->data[C::GEN1::OFFSETS::POKEDEX_OWNED];
	this->pokedex_seen = &this->m_rom->data[C::GEN1::OFFSETS::POKEDEX_SEEN];
	this->money = &this->m_rom->data[C::GEN1::OFFSETS::MONEY];
	this->rival_name = &this->m_rom->data[C::GEN1::OFFSETS::RIVAL_NAME];
	this->options = &this->m_rom->data[C::GEN1::OFFSETS::OPTIONS];
	this->badges = &this->m_rom->data[C::GEN1::OFFSETS::BADGES];
	this->player_trainer_id = (std::uint16_t*)&this->m_rom->data[C::GEN1::OFFSETS::PLAYER_TRAINER_ID];
	this->pikachu_friendship = &this->m_rom->data[C::GEN1::OFFSETS::PIKACHU_FRIENDSHIP];
	this->current_pc_box = &this->m_rom->data[C::GEN1::OFFSETS::CURRENT_PC_BOX];
	this->casino_coins = &this->m_rom->data[C::GEN1::OFFSETS::CASINO_COINS];
	this->current_box_list = &this->m_rom->data[C::GEN1::OFFSETS::CURRENT_BOX_LIST];
	this->checksum = &this->m_rom->data[C::GEN1::OFFSETS::CHECKSUM];

	this->pocket_items = (struct Gen1Structs::items_bag*)&this->m_rom->data[C::GEN1::OFFSETS::POCKET_ITEM_LIST];
	this->pc_item_list = (struct Gen1Structs::items_pc*)&this->m_rom->data[C::GEN1::OFFSETS::PC_ITEM_LIST];
	this->time_played = (struct Gen1Structs::pkmn_time*)&this->m_rom->data[C::GEN1::OFFSETS::TIME_PLAYED];
	this->team_pokemon_list = (struct Gen1Structs::pkmn_data_struct*)&this->m_rom->data[C::GEN1::OFFSETS::TEAM_POKEMON_LIST];

	auto offset = C::GEN1::OFFSETS::PC_BOX_1_POKEMON_LIST;
	for (auto i = 0; i < 12; i++) {
		this->pc_box[i] = (struct Gen1Structs::pkmn_box*)&this->m_rom->data[offset];
		offset += (sizeof(struct Gen1Structs::pkmn_box) * i);
	}
}

// NOTE: can probably be reused for any other names?
auto Gen1::get_player_name() const -> std::string
{
	if (this->player_name == nullptr) {
		return nullptr;
	}

	std::string name = "";
	for (auto i = 0; i < C::GEN1::SIZES::PLAYER_NAME; i++) {
        if(this->player_name[i] == 'P') {
			break;
		}

        name += C::GEN1::FONT[this->player_name[i]];
    }

    return name;
}

auto Gen1::set_player_name(std::string name) -> void
{
	std::size_t size = name.size();

	if (this->player_name == nullptr || name.empty()) {
		return;
	}

	if (size >= 7) {
		size = 7;
	}

	for (auto i = 0; i < size; i++) {
		this->player_name[i] = this->get_character_code(name[i]);
	}

	this->player_name[++size] = 'P';

	while (size < 11) {
		this->player_name[++size] = 0x0;
	}
}

auto Gen1::get_pokedex_owned(std::uint8_t index) const -> bool
{
	if (this->pokedex_owned && index < 152) {
		return (this->pokedex_owned[index >> 3] >> (index & 7) & 1) == 1;
	}

	return false;
}

auto Gen1::get_pokedex_seen(std::uint8_t index) const -> bool
{
	if (this->pokedex_seen && index < 152) {
		return (this->pokedex_seen[index >> 3] >> (index & 7) & 1) == 1;
	}

	return false;
}

auto Gen1::set_pokedex_owned(std::uint8_t index, bool owned) -> void
{
    if (owned) {
        this->pokedex_owned[index >> 3] |= 1 << (index & 7);
    } else {
		this->pokedex_owned[index >> 3] &= ~(1 << (index & 7));
	}
}

auto Gen1::set_pokedex_seen(std::uint8_t index, bool seen) -> void
{
    if (seen) {
        this->pokedex_seen[index >> 3] |= 1 << (index & 7);
    } else {
		this->pokedex_seen[index >> 3] &= ~(1 << (index & 7));
	}
}

auto Gen1::get_character_code(std::uint8_t const c) const -> std::uint8_t
{
    int i;

    for (i = 0; i < C::GEN1::SIZES::FONT; i++) {
        if (c == C::GEN1::FONT[i]) {
            return i;
        }
    }

    // return space code, as in, invalid character
    return 0x7F;
}
