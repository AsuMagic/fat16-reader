#include "bootsector.hpp"

#include <iostream>
#include "../log/logger.hpp"

FATBootSector::FATBootSector(std::ifstream& dump, const size_t start_sector)
{
	fatlog << trivial << "Lecture du secteur " << start_sector << " d'amorçage " << std::endl;
	dump.seekg(static_cast<std::streamoff>(start_sector * 512));
	dump.read(reinterpret_cast<char*>(&data), sizeof(data));

	fatlog << (*this) << std::endl;
}

unsigned long FATBootSector::sector_count() const
{
	// Plain old FAT16 can use a max of 65535 sectors.
	// FAT16B can support significantly more. When FAT16B is activated, boot_sector.sectors_short is deactivated.
	return (data.sectors_short ? data.sectors_short : data.sectors_long);
}


std::ostream& operator<<(std::ostream& stream, const FATBootSector& part)
{
	stream << "Taille de secteur   : " << part.data.sector_size << " octets" << std::endl
		   << "Taille de cluster   : " << static_cast<int>(part.data.cluster_size) << " secteurs" << std::endl
		   << "Secteurs réservés   : " << part.data.reserved << std::endl
		   << "Nombre de FATs      : " << static_cast<int>(part.data.fat_count) << std::endl
		   << "Taille de la racine : " << part.data.root_directory_size << std::endl
		   << "Taille de piste     : " << part.data.track_size << " secteurs" << std::endl
		   << "Nombre de têtes     : " << part.data.head_count << std::endl
		   << "Secteurs            : " << part.sector_count() << std::endl
		   << "Secteurs cachés     : " << part.data.hidden_sectors << std::endl
		   << "Type de média       : " << static_cast<int>(part.data.media) << std::endl
		   << "Créateur du système : '" << std::string{part.data.oem_code, part.data.oem_code + 8} << '\'' << std::endl
		   << "Légende             : '" << std::string{part.data.label, part.data.label + 11} << '\'';

	return stream;
}