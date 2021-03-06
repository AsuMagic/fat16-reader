#include <iostream> // character streams
#include <fstream> // file streams
#include <cstdint> // defined-size integers
#include <cstddef> // EXIT_FAILURE etc

#include "fat/filesystem.hpp"
#include "mbr/mbr.hpp"
#include "log/logger.hpp"

constexpr const char* dump_path = "/dev/sdd";

int main()
{
	iolog << "Lecture du disque à '" << dump_path << "'" << std::endl;
	std::ifstream dump{dump_path, std::ios::binary | std::ios::in};

	if (!dump)
	{
		iolog << important << "Impossible d'ouvrir le fichier en lecture" << std::endl;
		return EXIT_FAILURE;
	}

	dump.exceptions(std::ios::failbit | std::ios::badbit);

	try
	{
		mbr::MBR mbr{dump};

		for (mbr::Partition& mbr_entry : mbr.partitions)
		{
			if (mbr_entry.data.type)
			{
				fat::Filesystem part{dump, mbr_entry.data.start_sector};
				//part.ls("FOLDER");
				//part.cat("FOLDER/FILE.TXT");
			}
		}
	}
	catch(const std::ifstream::failure& e)
	{
		iolog << important << "Erreur de lecture - " << e.what() << std::endl;
		throw;
	}
}
