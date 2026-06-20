#include <iostream>
#include <string>
#include <iomanip>

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Bit.hpp"

int testsPassed = 0;
int testsFailed = 0;

void printTestResult(const std::string& testName, bool passed) {
    const int width = 80;
    std::cout << std::left << " - " << std::setw(width) << testName << " [";
    if (passed) {
        std::cout << "PASSED";
    } else {
        std::cout << "FAILED";
    }
    std::cout << "]" << std::endl;
}

template <typename T>

void assertEqual(const T& actual, const T& expected, const std::string& testName) {
	if (actual == expected) {
		printTestResult(testName, true);
		++testsPassed;
	} else {
		printTestResult(testName + " (expected: " + std::to_string(expected) + ", actual: " + std::to_string(actual) + ")", false);
		++testsFailed;
	}
}

namespace DynamicArrayTests
{
	void testDefaultConstructor() {
		DynamicArray<int> arr;
		assertEqual(arr.getSize(), 0, "Default constructor creates empty array");
	}

	void testSizeConstructor() {
	
	//Positive size:
	
		DynamicArray<int> arrPos(5);
		assertEqual(arrPos.getSize(),5, "Size constructor creates array of positive size");

	//Zero size:

		DynamicArray<int> arrNull(0);
		assertEqual(arrNull.getSize(), 0, "Size constructor creates array of null size");

	//Negative size:

		try {
        		DynamicArray<int> arrNeg(-5);
        		printTestResult("Size constructor negative size did not throw", false);
        		++testsFailed;
		} catch (const std::invalid_argument&) {
        		printTestResult("Size constructor negative size throws invalid_argument", true);
        		++testsPassed;
		} catch (...) {
        		printTestResult("Size constructor negative size threw wrong exception", false);
        		++testsFailed;
		}
	}

	void testConstructorFromArray() {

		int source[] = {1, 2, 3, 4, 5};

	//Positive amount:

		DynamicArray<int> arrPos(source, 5);
		assertEqual(arrPos.getSize(), 5, "Constructor from array with positive size");
		assertEqual(arrPos.get(0), 1, "Constructor from array with positive size: first element");
		assertEqual(arrPos.get(4), 5, "Constructor from array with positive size: last element");

	//Negative amount:

		try {
			DynamicArray<int> arrNeg(source, -5);
			printTestResult("Constructor from array with negative size did not throw", false);
			++testsFailed;
		} catch (const std::invalid_argument&) {
			printTestResult("Constructor from array with negative size throws exception", true);
			++testsPassed;
		} catch (...) {
			printTestResult("Constructor from array with negative size threw wrong exception", false);
			++testsFailed;
		}

	//Zero amount:

		DynamicArray<int> arrZero(source, 0);
		assertEqual(arrZero.getSize(), 0, "Constructor from array with zero size");


	//NullPtr:

		try {
        		DynamicArray<int> arrNullPtr(nullptr, 5);
        		printTestResult("Constructor from array with null pointer did not throw", false);
        		++testsFailed;
		} catch (const std::invalid_argument&) {
        		printTestResult("Constructor from array with null pointer throws exception", true);
        		++testsPassed;
		} catch (...) {
			printTestResult("Constructor from array with null pointer threw wrong exception", false);
			++testsFailed;
		}
	}

	void testCopyConstructor() {
		int source[] = {10, 20, 30};
		DynamicArray<int> arr1(source, 3);
		DynamicArray<int> arr2(arr1);

		assertEqual(arr2.getSize(), 3, "Copy constructor: size");
		assertEqual(arr2.get(0), 10, "Copy constructor: first element");
		assertEqual(arr2.get(2), 30, "Copy constructor: last element");

		arr2.set(0, 999);
		assertEqual(arr1.get(0), 10, "Copy constructor: deep copy (original unchanged)");
	}

	void testAssignmentOperator() {
		int source1[] = {1, 2, 3};
		int source2[] = {10, 20, 30, 40};
		DynamicArray<int> arr1(source1, 3);
		DynamicArray<int> arr2(source2, 4);

		arr2 = arr1;

		assertEqual(arr2.getSize(), 3, "Assignment: size after assignment");
		assertEqual(arr2.get(0), 1, "Assignment: first element after assignment");
		assertEqual(arr2.get(2), 3, "Assignment: last element after assignment");

		arr2.set(0, 999);
		assertEqual(arr1.get(0), 1, "Assignment: deep copy (original unchanged)");
	}

	void testGetSet() {

	//Default:

		DynamicArray<int> arr(3);
		arr.set(0, 10);
		arr.set(1, 20);
		arr.set(2, 30);

		assertEqual(arr.get(0), 10, "get/set: first element");
		assertEqual(arr.get(1), 20, "get/set: second element");
		assertEqual(arr.get(2), 30, "get/set: third element");

	//Get out of range:
		
		try {
			arr.get(5);
			printTestResult("Get out of range did not throw exception", false);
        		++testsFailed;
		} catch (const std::out_of_range&) {
			printTestResult("Get out of range threw exception", true);
        		++testsPassed;
		}

	//Set out of range:
	
		try {
			arr.set(5, 555);
			printTestResult("Set out of range did not throw exception", false);
	        	++testsFailed;
		} catch (const std::out_of_range&) {
			printTestResult("Set out of range threw exception", true);
	        	++testsPassed;
		}
	}

	void testResize() {

		DynamicArray<int> arr(3);
		arr.set(0, 1);
		arr.set(1, 2);
		arr.set(2, 3);

	//Increase:

		arr.resize(5);
		assertEqual(arr.getSize(), 5, "Resize(increase): size");
		assertEqual(arr.get(0), 1, "Resize(increase): first element preserved");
		assertEqual(arr.get(2), 3, "Resize(increase): last old element preserved");

	//Decrease:
	
		arr.resize(2);
		assertEqual(arr.getSize(), 2, "Resize(decrease): size");
		assertEqual(arr.get(0), 1, "Resize(decrease): first element preserved");
		assertEqual(arr.get(1), 2, "Resize(decrease): last kept element preserved");

	//Negative:

		try {
        		arr.resize(-5);
        		printTestResult("Resize(negative size) did not throw exception", false);
			++testsFailed;
		} catch (const std::invalid_argument&) {
			printTestResult("Resize negative size throws invalid_argument", true);
			++testsPassed;
		} catch(...) {
			printTestResult("Resize(negative size) threw wrong exception", false);
			++testsFailed;
		}

	//Removed elements:
	
		try {
        		arr.get(2);
			printTestResult("Resize: no out-of-range for removed elements received", false);
			++testsFailed;
		} catch (const std::out_of_range&) {
			printTestResult("Resize: out-of-range for removed elements received", true);
			++testsPassed;
		}

	//Zero:

		arr.resize(0);
		assertEqual(arr.getSize(), 0, "Resize(zero): size");
	}		
} // namespace DynamicArrayTests




namespace LinkedListTests {

	void testGetFirstLast() {
		int source[] = {10, 20, 30, 40};
		LinkedList<int> list(source, 4);

		assertEqual(list.getFirst(), 10, "Get first element");
		assertEqual(list.getLast(), 40, "Get last element");
		assertEqual(list.get(2), 30, "Get element with index 2");

		try {
			list.get(1000);
        		printTestResult("Get out of range did not throw", false);
        		++testsFailed;
		} catch (const std::out_of_range&) {
        		printTestResult("Get out of range throws exception", true);
        		++testsPassed;
		}

		LinkedList<int> listEmpty;

		try {
			listEmpty.getFirst();
        		printTestResult("GetFirst on empty list did not throw", false);
        		++testsFailed;
		} catch (const std::out_of_range&) {
        		printTestResult("GetFirst on empty list throws exception", true);
        		++testsPassed;
		}

		try {
			listEmpty.getLast();
        		printTestResult("GetLast on empty list did not throw", false);
        		++testsFailed;
		} catch (const std::out_of_range&) {
        		printTestResult("GetLast on empty list throws exception", true);
        		++testsPassed;
		}
	}

	void testDefaultConstructor() {
    		LinkedList<int> list;
		assertEqual(list.getLength(), 0, "Default constructor creates empty list");
	}

	void testConstructorFromArray() {

		int source[] = {10, 20, 30, 40, 50};

	//Positive amount:

		LinkedList<int> listPos(source, 5);
		assertEqual(listPos.getLength(), 5, "Constructor from array with positive size");
		assertEqual(listPos.getFirst(), 10, "Constructor from array with positive size: first element");
		assertEqual(listPos.getLast(), 50, "Constructor from array with positive size: last element");
	
	//Negative amount: 
		
		try {
			LinkedList<int> listNeg(source, -5);
        		printTestResult("Constructor from array with negative size did not throw", false);
        		++testsFailed;
		} catch (const std::invalid_argument&) {
        		printTestResult("Constructor from array with negative size throws exception", true);
			++testsPassed;
		} catch (...) {
        		printTestResult("Constructor from array with negative size threw wrong exception", false);
        		++testsFailed;
		}
	
	//Zero amount:

		LinkedList<int> listZero(source, 0);
		assertEqual(listZero.getLength(), 0, "Constructor from array with zero size");

	//Null pointer:

		try {
        		LinkedList<int> list(nullptr, 5);
        		printTestResult("Constructor from array with null pointer did not throw", false);
        		++testsFailed;
    		} catch (const std::invalid_argument&) {
        		printTestResult("Constructor from array with null pointer throws exception", true);
        		++testsPassed;
		} catch (...) {
        		printTestResult("Constructor from array with null pointer threw wrong exception", false);
        		++testsFailed;
		}
	}

	void testCopyConstructor() {
		int source[] = {1, 2, 3};
		LinkedList<int> list1(source, 3);
		LinkedList<int> list2(list1);

		assertEqual(list2.getLength(), 3, "Copy constructor: size");
		assertEqual(list2.getFirst(), 1, "Copy constructor: first element");
		assertEqual(list2.getLast(), 3, "Copy constructor: last element");

		list2.prepend(999);
		assertEqual(list1.getFirst(), 1, "Copy constructor: deep copy (original unchanged)");
	}

	void testAssignmentOperator() {
		int source1[] = {1, 2, 3};
		int source2[] = {10, 20, 30, 40};
		LinkedList<int> list1(source1, 3);
		LinkedList<int> list2(source2, 4);

		list2 = list1;
		
		assertEqual(list2.getLength(), 3, "Assignment: size after assignment");
		assertEqual(list2.getFirst(), 1, "Assignment: first element after assignment");
		assertEqual(list2.getLast(), 3, "Assignment: last element after assignment");

		list2.prepend(999);
		assertEqual(list1.getFirst(), 1, "Assignment: deep copy (original unchanged)");
	}

	void testAppendPrepend() {
		LinkedList<int> list;

		list.append(10);
		list.append(20);
		list.prepend(5);
		list.prepend(1);

		assertEqual(list.getLength(), 4, "Append/prepend: size");
		assertEqual(list.getFirst(), 1, "Append/prepend: first element after prepend");
		assertEqual(list.getLast(), 20, "Append/prepend: last element after append");
		assertEqual(list.get(1), 5, "Append/prepend: element at index 1");
		assertEqual(list.get(2), 10, "Append/prepend: element at index 2");
	}

	void testInsertAt() {
		int source[] = {1, 2, 4, 5};
		LinkedList<int> list(source, 4);

		list.insertAt(3, 2);
		list.insertAt(0, 0);
		list.insertAt(6, list.getLength());

		assertEqual(list.getLength(), 7, "InsertAt: size");
		assertEqual(list.getFirst(), 0, "InsertAt: first element after insert at 0");
		assertEqual(list.get(2), 2, "InsertAt: element at index 2");
		assertEqual(list.get(3), 3, "InsertAt: element at index 3");
		assertEqual(list.getLast(), 6, "InsertAt: last element after insert at end");

		try {
        		list.insertAt(99, 10);
        		printTestResult("InsertAt out-of-range did not throw", false);
			++testsFailed;
		} catch (const std::out_of_range&) {
        		printTestResult("InsertAt out-of-range throws exception", true);
			++testsPassed;
		} catch (...) {
        		printTestResult("InsertAt out-of-range threw wrong exception", false);
			++testsFailed;
		}
	}

	void testRemoveAt() {
		int source[] = {10, 20, 30, 40, 50};
		LinkedList<int> list(source, 5);

		list.removeAt(2); 
		assertEqual(list.getLength(), 4, "RemoveAt: size after removing middle");
		assertEqual(list.get(1), 20, "RemoveAt: element at index 1 after removal");
		assertEqual(list.get(2), 40, "RemoveAt: element at index 2 after removal");

		list.removeAt(0);
		assertEqual(list.getLength(), 3, "RemoveAt: size after removing head");
		assertEqual(list.getFirst(), 20, "RemoveAt: new head after removing head");

		list.removeAt(list.getLength() - 1);
		assertEqual(list.getLength(), 2, "RemoveAt: size after removing tail");
		assertEqual(list.getLast(), 40, "RemoveAt: new tail after removing tail");

		LinkedList<int> single;
    		single.append(100);
    		single.removeAt(0);
    		assertEqual(single.getLength(), 0, "RemoveAt: size after removing only element");
	
		try { 
			list.removeAt(-5);
			printTestResult("RemoveAt negative index did not throw", false);
			++testsFailed;
		} catch (const std::out_of_range&) {
			printTestResult("RemoveAt negative index throws exception", true);
			++testsPassed;
		}

		try { 
			list.removeAt(1000);
			printTestResult("RemoveAt index out of range did not throw", false);
			++testsFailed;
		} catch (const std::out_of_range&) {
			printTestResult("RemoveAt index out of range throws exception", true);
			++testsPassed;
		}

	}

	void testGetSublist() {

		int source[] = {1, 2, 3, 4, 5, 6};
		LinkedList<int> list(source, 6);
		LinkedList<int> sub = list.getSublist(1, 3);

		assertEqual(sub.getLength(), 3, "GetSublist: size");
		assertEqual(sub.getFirst(), 2, "GetSublist: first element");
		assertEqual(sub.getLast(), 4, "GetSublist: last element");
		assertEqual(sub.get(1), 3, "GetSublist: element at index 1");

		try {
        		list.getSublist(-1, 2);
        		printTestResult("GetSublist negative start did not throw exception", false);
			++testsFailed;
		} catch (const std::out_of_range&) {
        		printTestResult("GetSublist negative start throws exception", true);
        		++testsPassed;
		} catch (...) {
        		printTestResult("GetSublist negative start threw wrong exception", false);
			++testsFailed;
		}

		try {
        		list.getSublist(2, 10);
        		printTestResult("GetSublist end out of range did not throw exception", false);
			++testsFailed;
		} catch (const std::out_of_range&) {
        		printTestResult("GetSublist end out of range throws exception", true);
        		++testsPassed;
		} catch (...) {
        		printTestResult("GetSublist end out of range threw wrong exception", false);
			++testsFailed;
		}

		try {
			list.getSublist(4, 2);
        		printTestResult("GetSublist start > end did not throw", false);
        		++testsFailed;
		} catch (const std::out_of_range&) {
        		printTestResult("GetSublist start > end throws", true);
        		++testsPassed;
		} catch (...) {
        		printTestResult("GetSublist start > end threw wrong exception", false);
			++testsFailed;
		}
	}

	void testLinkedListConcat() {
		int source1[] = {1, 2, 3};
		int source2[] = {4, 5, 6};
		LinkedList<int> list1(source1, 3);
    		LinkedList<int> list2(source2, 3);

		LinkedList<int> result = list1.concat(list2);
		assertEqual(result.getLength(), 6, "LinkedList concat: size");
		assertEqual(result.getFirst(), 1, "LinkedList concat: first element");
		assertEqual(result.getLast(), 6, "LinkedList concat: last element");
		assertEqual(result.get(2), 3, "LinkedList concat: element at index 2");
		assertEqual(result.get(3), 4, "LinkedList concat: element at index 3");
	}
} // namespace LinkedListTests

namespace ArraySequenceTests {

	void testMutableCreation() {
        	MutableArraySequence<int> seq;
        	assertEqual(seq.getLength(), 0, "MutableArraySequence default constructor");
        
        	int arr[] = {1, 2, 3};
        	MutableArraySequence<int> seq2(arr, 3);
        	assertEqual(seq2.getLength(), 3, "MutableArraySequence from array");
        	assertEqual(seq2.getFirst(), 1, "MutableArraySequence getFirst");
        	assertEqual(seq2.getLast(), 3, "MutableArraySequence getLast");
	}

	void testImmutableCreation() {
        	ImmutableArraySequence<int> seq;
        	assertEqual(seq.getLength(), 0, "ImmutableArraySequence default constructor");
        
        	int arr[] = {1, 2, 3};
        	ImmutableArraySequence<int> seq2(arr, 3);
        	assertEqual(seq2.getLength(), 3, "ImmutableArraySequence from array");
        	assertEqual(seq2.getFirst(), 1, "ImmutableArraySequence getFirst");
        	assertEqual(seq2.getLast(), 3, "ImmutableArraySequence getLast");
	}

	void testMutableAppendPrependInsertRemove() {
        	MutableArraySequence<int> seq;
        	seq.append(10);
        	seq.append(20);
        	seq.prepend(5);
        	seq.insertAt(15, 2);
        	seq.remove(1);

        	assertEqual(seq.getLength(), 3, "MutableArraySequence after operations");
        	assertEqual(seq.get(0), 5, "MutableArraySequence first element");
        	assertEqual(seq.get(1), 15, "MutableArraySequence second element");
		assertEqual(seq.get(2), 20, "MutableArraySequence third element");
    	}

	void testImmutableAppendPrependInsertRemove() {
        	ImmutableArraySequence<int> seq;
        
        	Sequence<int>* newSeq = seq.append(10);
        	assertEqual(newSeq->getLength(), 1, "ImmutableArraySequence append result length");
        	assertEqual(newSeq->get(0), 10, "ImmutableArraySequence append result value");
        	delete newSeq;

        	int arr[] = {1, 2, 3};
        	ImmutableArraySequence<int> seq2(arr, 3);
        	Sequence<int>* newSeq2 = seq2.prepend(0);
        	assertEqual(newSeq2->getLength(), 4, "ImmutableArraySequence prepend result length");
        	assertEqual(newSeq2->get(0), 0, "ImmutableArraySequence prepend result first");
        	assertEqual(seq2.getLength(), 3, "ImmutableArraySequence original unchanged after prepend");
        	delete newSeq2;

        	Sequence<int>* newSeq3 = seq2.insertAt(99, 1);
        	assertEqual(newSeq3->getLength(), 4, "ImmutableArraySequence insertAt result length");
        	assertEqual(newSeq3->get(1), 99, "ImmutableArraySequence insertAt result value at index 1");
        	delete newSeq3;

        	Sequence<int>* newSeq4 = seq2.remove(1);
        	assertEqual(newSeq4->getLength(), 2, "ImmutableArraySequence remove result length");
        	assertEqual(newSeq4->get(0), 1, "ImmutableArraySequence remove result first");
        	assertEqual(newSeq4->get(1), 3, "ImmutableArraySequence remove result second");
        	assertEqual(seq2.getLength(), 3, "ImmutableArraySequence original unchanged after remove");
        	delete newSeq4;
	}

	void testMutableGetSubsequence() {
        	int arr[] = {1, 2, 3, 4, 5};
        	MutableArraySequence<int> seq(arr, 5);
        	Sequence<int>* sub = seq.getSubsequence(1, 3);
        	assertEqual(sub->getLength(), 3, "MutableArraySequence getSubsequence length");
        	assertEqual(sub->get(0), 2, "MutableArraySequence getSubsequence first");
		assertEqual(sub->get(1), 3, "MutableArraySequence getSubsequence second");
		assertEqual(sub->get(2), 4, "MutableArraySequence getSubsequence third");
		delete sub;
    	}

	void testImmutableGetSubsequence() {
        	int arr[] = {1, 2, 3, 4, 5};
        	ImmutableArraySequence<int> seq(arr, 5);
        	Sequence<int>* sub = seq.getSubsequence(1, 3);
        	assertEqual(sub->getLength(), 3, "ImmutableArraySequence getSubsequence length");
        	assertEqual(sub->get(0), 2, "ImmutableArraySequence getSubsequence first");
        	assertEqual(seq.getLength(), 5, "ImmutableArraySequence original unchanged after getSubsequence");
        	delete sub;
    	}

    	void testMutableConcat() {
       		int arr1[] = {1, 2};
        	int arr2[] = {3, 4};
        	MutableArraySequence<int> seq1(arr1, 2);
        	MutableArraySequence<int> seq2(arr2, 2);
        	Sequence<int>* result = seq1.concat(seq2);
        	assertEqual(result->getLength(), 4, "MutableArraySequence concat length");
        	assertEqual(result->get(0), 1, "MutableArraySequence concat first");
        	assertEqual(result->get(1), 2, "MutableArraySequence concat second");
        	assertEqual(result->get(2), 3, "MutableArraySequence concat third");
        	assertEqual(result->get(3), 4, "MutableArraySequence concat fourth");
        	delete result;
    	}

	void testImmutableConcat() {
        	int arr1[] = {1, 2};
        	int arr2[] = {3, 4};
        	ImmutableArraySequence<int> seq1(arr1, 2);
        	ImmutableArraySequence<int> seq2(arr2, 2);
        	Sequence<int>* result = seq1.concat(seq2);
        	assertEqual(result->getLength(), 4, "ImmutableArraySequence concat length");
        	assertEqual(result->get(0), 1, "ImmutableArraySequence concat first");
        	assertEqual(seq1.getLength(), 2, "ImmutableArraySequence original unchanged after concat");
        	delete result;
    	}

	void testExceptions() {
        	MutableArraySequence<int> seq;
        	try {
        		seq.getFirst();
        		printTestResult("getFirst on empty MutableArraySequence did not throw", false);
        		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("getFirst on empty MutableArraySequence throws", true);
        		++testsPassed;
        	}

        	try {
        		seq.getLast();
        		printTestResult("getLast on empty MutableArraySequence did not throw", false);
        		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("getLast on empty MutableArraySequence throws", true);
        		++testsPassed;
        	}

        	try {
        		seq.get(0);
        		printTestResult("get on empty MutableArraySequence did not throw", false);
        		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("get on empty MutableArraySequence throws", true);
        		++testsPassed;
        	}

        	try {
        		seq.insertAt(5, -1);
        		printTestResult("insertAt with negative index did not throw", false);
        		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("insertAt with negative index throws", true);
        		++testsPassed;
        	}

        	try {
        		seq.insertAt(5, 10);
        		printTestResult("insertAt with index > length did not throw", false);
        		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("insertAt with index > length throws", true);
        		++testsPassed;
        	}

        	try {
        		seq.remove(0);
        		printTestResult("remove on empty MutableArraySequence did not throw", false);
        		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("remove on empty MutableArraySequence throws", true);
        		++testsPassed;
        	}
	}
} // namespace ArraySequenceTests

namespace ListSequenceTests {

	void testMutableCreation() {
	        MutableListSequence<int> seq;
        	assertEqual(seq.getLength(), 0, "MutableListSequence default constructor");
        
        	int arr[] = {1, 2, 3};
        	MutableListSequence<int> seq2(arr, 3);
        	assertEqual(seq2.getLength(), 3, "MutableListSequence from array");
        	assertEqual(seq2.getFirst(), 1, "MutableListSequence getFirst");
        	assertEqual(seq2.getLast(), 3, "MutableListSequence getLast");
    	}

	void testImmutableCreation() {
        	ImmutableListSequence<int> seq;
        	assertEqual(seq.getLength(), 0, "ImmutableListSequence default constructor");
        
        	int arr[] = {1, 2, 3};
        	ImmutableListSequence<int> seq2(arr, 3);
        	assertEqual(seq2.getLength(), 3, "ImmutableListSequence from array");
        	assertEqual(seq2.getFirst(), 1, "ImmutableListSequence getFirst");
        	assertEqual(seq2.getLast(), 3, "ImmutableListSequence getLast");
    	}

	void testMutableAppendPrependInsertRemove() {
        	MutableListSequence<int> seq;
        	seq.append(10);
        	seq.append(20);
        	seq.prepend(5);
        	seq.insertAt(15, 2);
        	seq.remove(1);

        	assertEqual(seq.getLength(), 3, "MutableListSequence after operations");
        	assertEqual(seq.get(0), 5, "MutableListSequence first element");
        	assertEqual(seq.get(1), 15, "MutableListSequence second element");
        	assertEqual(seq.get(2), 20, "MutableListSequence third element");
    	}

	void testImmutableAppendPrependInsertRemove() {
        	ImmutableListSequence<int> seq;
        	assertEqual(seq.getLength(), 0, "ImmutableListSequence original unchanged after append");
        
       		Sequence<int>* newSeq = seq.append(10);
        	assertEqual(newSeq->getLength(), 1, "ImmutableListSequence append result length");
        	assertEqual(newSeq->get(0), 10, "ImmutableListSequence append result value");
        	delete newSeq;

        	int arr[] = {1, 2, 3};
        	ImmutableListSequence<int> seq2(arr, 3);
        
        	Sequence<int>* newSeq2 = seq2.prepend(0);
        	assertEqual(newSeq2->getLength(), 4, "ImmutableListSequence prepend result length");
        	assertEqual(newSeq2->get(0), 0, "ImmutableListSequence prepend result first");
        	assertEqual(seq2.getLength(), 3, "ImmutableListSequence original unchanged after prepend");
        	delete newSeq2;
	
        	Sequence<int>* newSeq3 = seq2.insertAt(99, 1);
        	assertEqual(newSeq3->getLength(), 4, "ImmutableListSequence insertAt result length");
        	assertEqual(newSeq3->get(1), 99, "ImmutableListSequence insertAt result value");
        	delete newSeq3;

        	Sequence<int>* newSeq4 = seq2.remove(1);
        	assertEqual(newSeq4->getLength(), 2, "ImmutableListSequence remove result length");
        	assertEqual(newSeq4->get(0), 1, "ImmutableListSequence remove result first");
        	assertEqual(newSeq4->get(1), 3, "ImmutableListSequence remove result second");
        	assertEqual(seq2.getLength(), 3, "ImmutableListSequence original unchanged after remove");
        	delete newSeq4;
    	}

	void testMutableGetSubsequence() {
        	int arr[] = {1, 2, 3, 4, 5};
        	MutableListSequence<int> seq(arr, 5);
        	Sequence<int>* sub = seq.getSubsequence(1, 3);
        	assertEqual(sub->getLength(), 3, "MutableListSequence getSubsequence length");
        	assertEqual(sub->get(0), 2, "MutableListSequence getSubsequence first");
        	assertEqual(sub->get(1), 3, "MutableListSequence getSubsequence second");
        	assertEqual(sub->get(2), 4, "MutableListSequence getSubsequence third");
        	delete sub;
    	}

	void testImmutableGetSubsequence() {
        	int arr[] = {1, 2, 3, 4, 5};
        	ImmutableListSequence<int> seq(arr, 5);
        	Sequence<int>* sub = seq.getSubsequence(1, 3);
        	assertEqual(sub->getLength(), 3, "ImmutableListSequence getSubsequence length");
        	assertEqual(sub->get(0), 2, "ImmutableListSequence getSubsequence first");
        	assertEqual(seq.getLength(), 5, "ImmutableListSequence original unchanged after getSubsequence");
        	delete sub;
    	}

    	void testMutableConcat() {
        	int arr1[] = {1, 2};
        	int arr2[] = {3, 4};
        	MutableListSequence<int> seq1(arr1, 2);
        	MutableListSequence<int> seq2(arr2, 2);
        	Sequence<int>* result = seq1.concat(seq2);
        	assertEqual(result->getLength(), 4, "MutableListSequence concat length");
        	assertEqual(result->get(0), 1, "MutableListSequence concat first");
        	assertEqual(result->get(1), 2, "MutableListSequence concat second");
        	assertEqual(result->get(2), 3, "MutableListSequence concat third");
        	assertEqual(result->get(3), 4, "MutableListSequence concat fourth");
        	delete result;
    	}

    	void testImmutableConcat() {
        	int arr1[] = {1, 2};
        	int arr2[] = {3, 4};
        	ImmutableListSequence<int> seq1(arr1, 2);
        	ImmutableListSequence<int> seq2(arr2, 2);
        	Sequence<int>* result = seq1.concat(seq2);
        	assertEqual(result->getLength(), 4, "ImmutableListSequence concat length");
        	assertEqual(result->get(0), 1, "ImmutableListSequence concat first");
        	assertEqual(seq1.getLength(), 2, "ImmutableListSequence original unchanged after concat");
        	delete result;
    	}

    	void testExceptions() {
        	MutableListSequence<int> seq;
        	try {
        		seq.getFirst();
        		printTestResult("getFirst on empty MutableListSequence did not throw", false);
        		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("getFirst on empty MutableListSequence throws", true);
        		++testsPassed;
        	}

        	try {
            		seq.getLast();
            		printTestResult("getLast on empty MutableListSequence did not throw", false);
            		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("getLast on empty MutableListSequence throws", true);
            		++testsPassed;
        	}

        	try {
            		seq.get(0);
            		printTestResult("get on empty MutableListSequence did not throw", false);
            		++testsFailed;
        	} catch (const std::out_of_range&) {
            		printTestResult("get on empty MutableListSequence throws", true);
            		++testsPassed;
        	}

        	try {
            		seq.insertAt(5, -1);
            		printTestResult("insertAt negative index did not throw", false);
            		++testsFailed;
        	} catch (const std::out_of_range&) {
            		printTestResult("insertAt negative index throws", true);
            		++testsPassed;
        	}

        	try {
            		seq.insertAt(5, 10);
            		printTestResult("insertAt index > length did not throw", false);
            		++testsFailed;
        	} catch (const std::out_of_range&) {
            		printTestResult("insertAt index > length throws", true);
            		++testsPassed;
        	}

        	try {
            		seq.remove(0);
            		printTestResult("remove on empty MutableListSequence did not throw", false);
            		++testsFailed;
        	} catch (const std::out_of_range&) {
        		printTestResult("remove on empty MutableListSequence throws", true);
        		++testsPassed;
        	}
    	}
} // namespace ListSequenceTests

namespace BitTests {

	void testConstructors() {
        	Bit b1;
        	assertEqual(b1.get(), false, "Bit default constructor: false");

        	Bit b2(true);
        	assertEqual(b2.get(), true, "Bit bool constructor: true");

        	Bit b3(false);
        	assertEqual(b3.get(), false, "Bit bool constructor: false");

        	Bit b4(1);
        	assertEqual(b4.get(), true, "Bit int constructor: 1 -> true");

        	Bit b5(0);
        	assertEqual(b5.get(), false, "Bit int constructor: 0 -> false");

        	Bit b6(42);
        	assertEqual(b6.get(), true, "Bit int constructor: non-zero -> true");
    	}

	void testOperators() {
        	Bit a(true), b(false);

        	Bit andResult = a & b;
        	assertEqual(andResult.get(), false, "AND: true & false = false");

        	Bit orResult = a | b;
        	assertEqual(orResult.get(), true, "OR: true | false = true");

        	Bit xorResult = a ^ b;
        	assertEqual(xorResult.get(), true, "XOR: true ^ false = true");

        	Bit notA = ~a;
        	assertEqual(notA.get(), false, "NOT: ~true = false");

        	Bit notB = ~b;
        	assertEqual(notB.get(), true, "NOT: ~false = true");

        	assertEqual(a == b, false, "Equality: true == false -> false");
        	assertEqual(a != b, true, "Inequality: true != false -> true");

        	Bit c(true);
        	assertEqual(a == c, true, "Equality: true == true -> true");
        	assertEqual(a != c, false, "Inequality: true != true -> false");
	}
} // namespace BitTests

void runAllTests() {
	std::cout << "====== TESTS ======" << std::endl;

	std::cout << "\nDynamicArray:\n" << std::endl;
	{
		using namespace DynamicArrayTests;	
		testDefaultConstructor();
		testSizeConstructor();
		testConstructorFromArray();
		testCopyConstructor();
		testAssignmentOperator();
		testGetSet();
		testResize();
	}

	std::cout << "\nLinkedList:\n" << std::endl;
	{
		using namespace LinkedListTests;
		testGetFirstLast();
		testDefaultConstructor();
		testConstructorFromArray();
		testCopyConstructor();
		testAssignmentOperator();
		testAppendPrepend();
		testInsertAt();
		testRemoveAt();
		testGetSublist();
		testLinkedListConcat();
	}

	std::cout << "\nArraySequence:\n" << std::endl;
	{
		using namespace ArraySequenceTests;
		testMutableCreation();
		testImmutableCreation();
		testMutableAppendPrependInsertRemove();
		testImmutableAppendPrependInsertRemove();
		testMutableGetSubsequence();
		testImmutableGetSubsequence();
		testMutableConcat();
		testImmutableConcat();
		testExceptions();
	}

	std::cout << "\nListSequence:\n" << std::endl;
	{
		using namespace ListSequenceTests;
    		testMutableCreation();
    		testImmutableCreation();
    		testMutableAppendPrependInsertRemove();
    		testImmutableAppendPrependInsertRemove();
    		testMutableGetSubsequence();
    		testImmutableGetSubsequence();
    		testMutableConcat();
    		testImmutableConcat();
    		testExceptions();
	}

	std::cout << "\nBit:\n" << std::endl;
	{
    		using namespace BitTests;
    		testConstructors();
    		testOperators();
	}

	std::cout << "\n=== Results ===" << std::endl;
	std::cout << testsPassed << "/" << testsPassed + testsFailed << " tests passed." << std::endl;
}