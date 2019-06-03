#ifndef FLATNTUPLEMASK_HPP
#define FLATNTUPLEMASK_HPP

// ROOT IMPORTS
#include <TChain.h>
#include <TString.h>

// CPP IMPORTS
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

namespace emtf {

	class FlatNTuple {
	public:
		FlatNTuple();

		~FlatNTuple();

		void setBranchAddresses(TChain* chain);

		long long I(const TString& str) const {
			if (mInts.find(str) != mInts.end()) {
				return (long long) mInts.at(str);
			} else if (mLongs.find(str) != mLongs.end()) {
				return mLongs.at(str);
			} else {
				std::cout << "\nString '" << str << "' not found in mInts ('ints') or mLongs ('longs')." << std::endl;
				throw std::invalid_argument("");
			}
		};

		float F(const TString& str, unsigned int idx) const {
			if (mVFlt.find(str) != mVFlt.end()) {
				if (mVFlt.at(str)->size() > idx) {
					return mVFlt.at(str)->at(idx);
				} else {
					std::cout << "\nVector '" << str << "' has " << mVFlt.at(str)->size() << " elements, not " << idx + 1 << "." << std::endl;
					throw std::invalid_argument("");
				}
			} else {
				std::cout << "\nString '" << str << "' not found in mVFlt ('vFlt')." << std::endl;
				throw std::invalid_argument("");
			}
		};

		int I(const TString& str, unsigned int idx) const {
			if (mVInt.find(str) != mVInt.end()) {
				if (mVInt.at(str)->size() > idx) {
					return mVInt.at(str)->at(idx);
				} else {
					std::cout << "\nVector '" << str << "' has " << mVInt.at(str)->size() << " elements, not " << idx + 1 << "." << std::endl;
					throw std::invalid_argument("");
				}
			} else {
				std::cout << "\nString '" << str << "' not found in mVInt ('vInt')." << std::endl;
				throw std::invalid_argument("");
			}
		};

		std::vector<float>* VF(const TString& str) const {
			if (mVFlt.find(str) != mVFlt.end()) {
				return mVFlt.at(str);
			} else {
				std::cout << "\nString '" << str << "' not found in mVFlt ('vFlt')." << std::endl;
				throw std::invalid_argument("");
			}
		};

		std::vector<int>* VI(const TString& str) const {
			if (mVInt.find(str) != mVInt.end()) {
				return mVInt.at(str);
			} else {
				std::cout << "\nString '" << str << "' not found in mVInt ('vInt')." << std::endl;
				throw std::invalid_argument("");
			}
		};

	private:
		std::map<TString, int> mInts;
		std::map<TString, long long> mLongs;
		std::map<TString, std::vector<float>*> mVFlt;
		std::map<TString, std::vector<int>*> mVInt;
	};
} // namespace emtf

#endif

