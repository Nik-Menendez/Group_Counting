// MY IMPORTS
#include "FlatNTupleEngine.hpp"

// ROOT IMPORTS
#include <TChain.h>

// CPP IMPORTS
#include <iostream>

namespace emtf {

FlatNTupleEngine::FlatNTupleEngine() {

}

FlatNTupleEngine::~FlatNTupleEngine() {

}

void FlatNTupleEngine::addRootFile(const std::string& file_path) {
	files.push_back(file_path);
}

void FlatNTupleEngine::forEachEvent(const std::function<void(unsigned int, const FlatNTuple&) >& consumer) const {
	TChain chain("FlatNtupleData/tree");
	const unsigned int file_count = files.size();

	std::cout << "****************************************" << std::endl;
	std::cout << "# Files to be processed: " << file_count << std::endl;
	std::cout << "****************************************" << std::endl;

	std::string file_path;
	for (unsigned int i = 0; i < file_count; i++) {
		file_path = files.at(i);

		std::cout << file_path << std::endl;

		chain.Add(file_path.c_str());
	}

	std::cout << "****************************************" << std::endl;

	const unsigned int event_count = chain.GetEntries();

	std::cout << "****************************************" << std::endl;
	std::cout << "# Events found: " << event_count << std::endl;
	std::cout << "****************************************" << std::endl;

	FlatNTuple ntuple;
	ntuple.setBranchAddresses(&chain);

	for (unsigned int i_event = 0; i_event < event_count; i_event++) {
		if ((i_event % 1000) == 0) {
			float percentage = ((float) i_event / (float) event_count) * 100.;
			std::cout << "# Progress: " << percentage << std::endl;
		}

		// GET NTUPLE
		chain.GetEntry(i_event);

		// CONSUME
		consumer(i_event, ntuple);
	}
}
} // namespace emtf

