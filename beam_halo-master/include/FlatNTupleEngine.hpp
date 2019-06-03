#ifndef FLATNTUPLEENGINE_HPP
#define FLATNTUPLEENGINE_HPP

// MY IMPORTS
#include "FlatNTuple.hpp"

// ROOT IMPORTS
#include <TString.h>

// CPP IMPORTS
#include <functional>
#include <string>
#include <vector>

namespace emtf {

	class FlatNTupleEngine {
	public:
		FlatNTupleEngine();
		~FlatNTupleEngine();

		void addRootFile(const std::string& file_path);

		void forEachEvent(const std::function<void(unsigned int, const FlatNTuple&) >& consumer) const;

	private:
		std::vector<std::string> files;
	};
} // namespace emtf

#endif

