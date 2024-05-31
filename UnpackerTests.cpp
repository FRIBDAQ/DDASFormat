/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2016.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Author:
             Jeromy Tompkins
	     Aaron Chester
	     Facility for Rare Isotope Beams
	     Michigan State University
	     East Lansing, MI 48824-1321
*/

#include <stdint.h>
#include <math.h>

#include <vector>
#include <array>
#include <tuple>

#include <cppunit/extensions/HelperMacros.h>

#include "Asserts.h"
#include "DDASHit.h"
#include "DDASHitUnpacker.h"

using namespace std;
using namespace ::DAQ::DDAS;

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec)
{
    stream << "{ ";
    for (auto& element : vec ) stream << element << " ";
    stream << "}";

    return stream;
}

template<class T, long unsigned int N>
std::ostream& operator<<(std::ostream& stream, const std::array<T,N>& vec)
{
    stream << "{ ";
    for (int i=0; i<N; ++i) stream << vec[i] << " ";
    stream << "}";

    return stream;
}

class UnpackerTests : public CppUnit::TestFixture
{
private:
    DDASHit hit100, hit250, hit500;

public:
    CPPUNIT_TEST_SUITE(UnpackerTests);
    
    CPPUNIT_TEST(crateID_100);
    CPPUNIT_TEST(slotID_100);
    CPPUNIT_TEST(chanID_100);
    CPPUNIT_TEST(headerLength_100);
    CPPUNIT_TEST(eventLength_100);
    CPPUNIT_TEST(finishCode_100);
    CPPUNIT_TEST(msps_100);
    CPPUNIT_TEST(timeLow_100);
    CPPUNIT_TEST(timeHigh_100);
    CPPUNIT_TEST(coarseTime_100);
    CPPUNIT_TEST(time_100);
    CPPUNIT_TEST(cfdFail_100);
    CPPUNIT_TEST(cfdTrigSource_100);
    CPPUNIT_TEST(energySums_100);
    CPPUNIT_TEST(qdcSums_100);
    CPPUNIT_TEST(trace_100);

    CPPUNIT_TEST(msps_250);
    CPPUNIT_TEST(revision_250);
    CPPUNIT_TEST(resolution_250);
    CPPUNIT_TEST(coarseTime_250);
    CPPUNIT_TEST(time_250);
    CPPUNIT_TEST(cfdFail_250);
    CPPUNIT_TEST(cfdTrigSource_250);

    CPPUNIT_TEST(msps_500);
    CPPUNIT_TEST(revision_500);
    CPPUNIT_TEST(resolution_500);
    CPPUNIT_TEST(coarseTime_500);
    CPPUNIT_TEST(time_500);
    CPPUNIT_TEST(cfdTrigSource_500);
    CPPUNIT_TEST(cfdFail_500_0);
    CPPUNIT_TEST(cfdFail_500_1);
    
    CPPUNIT_TEST(externalClock_0);
    CPPUNIT_TEST(externalClock_1);
    CPPUNIT_TEST(externalClock_2);
    CPPUNIT_TEST(externalClock_3);
    
    CPPUNIT_TEST_SUITE_END();

public:
    /** 
     * @brief Configure and unpack some sample hits.
     */
    void setUp()
	{
	    DDASHitUnpacker unpacker;

	    // 100 MSPS data:
	    
	    vector<uint32_t> data = {
		0x0000002c, 0x0c0c0064, 0x002d2321, 0x0000f687,
		0x947f000a, 0x000808be,	0x00000001, 0x00000002,
		0x00000003, 0x00000004, 0x00000005, 0x00000006,
		0x00000007, 0x00000008, 0x00000009, 0x0000000a,
		0x0000000b, 0x0000000c, 0x0a0a0b0b, 0x0c0c0d0d,
		0x00020001, 0x00040003, 0x00060005, 0x00080007
	    };
	    tie(hit100, ignore) = unpacker.unpack(
		data.data(), data.data()+data.size()
		);

	    // 250 MSPS data:
	    data[1] = 0x0f1000fa; // Module ID word
	    data[4] = 0x547f000a; // Upper 16 bits are CFD info
	    tie(hit250, ignore) = unpacker.unpack(
		data.data(), data.data()+data.size()
		);    
	    
	    // 500 MSPS data:
	    data[1] = 0x0f0e01f4; // Module ID word
	    data[4] = 0x747f000a; // Upper 16 bits are CFD info
	    tie(hit500, ignore) = unpacker.unpack(
		data.data(), data.data()+data.size()
		);    
	}

    /** @brief Delete new'd stuff here. */
    void tearDown() {}

    //_______________________________________________________________________
    // Tests for 100 MSPS modules + generic member data
    //
    
    /** @brief Check the crate ID value. */
    void crateID_100 ()
	{
	    EQMSG(
		"100 MSPS extract crate ID",
		uint32_t(3), hit100.getCrateID()
		); 
	}

    /** @brief Check the slot ID value. */
    void slotID_100 ()
	{
	    EQMSG(
		"100 MSPS extract slot ID",
		uint32_t(2), hit100.getSlotID()
		);
	}
    
    /** @brief Check the channel ID value. */
    void chanID_100 ()
	{
	    EQMSG(
		"100 MSPS extract channel ID",
		uint32_t(1), hit100.getChannelID()
		); 
	}

    /** @brief Check the number of header words. */
    void headerLength_100 ()
	{
	    EQMSG(
		"100 MSPS extract header length",
		uint32_t(18), hit100.getChannelHeaderLength()
		); 
	}

    /** @brief Check the event length in 32-bit words. */
    void eventLength_100 ()
	{
	    EQMSG(
		"100 MSPS extract event length",
		uint32_t(22), hit100.getChannelLength()
		); 
	}

    /** @brief Check the module finish code. */
    void finishCode_100 ()
	{
	    EQMSG(
		"100 MSPS extract finish code",
		uint32_t(0), hit100.getFinishCode()
		); 
	}

    /** @brief Check the module MSPS can be read. */
    void msps_100 ()
	{
	    EQMSG(
		"100 MSPS extract adc frequency",
		uint32_t(100), hit100.getModMSPS()
		); 
	}

    /** @brief Check lower 32 bits of 48 bit timestamp. */
    void timeLow_100 ()
	{
	    EQMSG(
		"100 MSPS extract time low",
		uint32_t(63111), hit100.getTimeLow()
		); 
	}

    /** @brief Check upper 16 bits of 48 bit timestamp. */
    void timeHigh_100 ()
	{
	    EQMSG(
		"100 MSPS extract time high",
		uint32_t(10), hit100.getTimeHigh()
		); 
	}

    /** @brief Check reconstructed coarse timestamp (in ns). */
    void coarseTime_100 ()
	{
	    EQMSG(
		"100 MSPS coarse time",
		uint64_t(0x000a0000f687)*10, hit100.getCoarseTime()
		); 
	}

    /** @brief Check corrected full timestamp (in ns). */
    void time_100 ()
	{
	    // The magic number is reconstructed TS for 100 MSPS:
	    ASSERTMSG(
		"100 MSPS full time",
		std::abs(hit100.getTime()-429497360711.601257) < 0.000001
		);
	}

    /** @brief Check that we can parse the CFD fail word. */
    void cfdFail_100 ()
	{
	    EQMSG(
		"100 MSPS compute CFD fail bit",
		uint32_t(1), hit100.getCFDFailBit()
		);
	}

    /** @brief Get the CFD trigger source bit. */
    void cfdTrigSource_100 ()
	{
	    EQMSG(
		"100 MSPS compute CFD trig source bit",
		uint32_t(0), hit100.getCFDTrigSource()
		);
	}

    /** @brief Read the energy sums. */
    void energySums_100 ()
	{
	    std::vector<uint32_t> expected = {1, 2, 3, 4};
	    EQMSG("Found all 4 energy sums", expected, hit100.getEnergySums());
	}

    /** @brief Read the QDC sums. */
    void qdcSums_100 ()
	{
	    std::vector<uint32_t> expected = {5, 6, 7, 8, 9, 10, 11, 12};
	    EQMSG("Found all 4 energy sums", expected, hit100.getQDCSums());
	}

    /** @brief Read trace data. */
    void trace_100 ()
	{
	    std::vector<uint16_t> expected = {1, 2, 3, 4, 5, 6, 7, 8};
	    EQMSG("Found all trace samples", expected, hit100.getTrace());
	}

    //_______________________________________________________________________
    // Tests for 250 MSPS modules
    //
    
    /** @brief Read MSPS from module identifier word for 250 MSPS. */
    void msps_250 ()
	{
	    EQMSG(
		"250 MSPS extract module MSPS",
		uint32_t(250), hit250.getModMSPS()
		); 
	}

    /** @brief Read revision from module idntifier word for 250 MSPS. */
    void revision_250 ()
	{
	    EQMSG(
		"250 MSPS extract hardware revision",
		15, hit250.getHardwareRevision()
		);
	}
    
    /** 
     * @brief Read ADC resolution (bit depth) from module identifier word 
     * for 16-bit 250 MSPS. 
     */
    void resolution_250 ()
	{
	    EQMSG(
		"250 MSPS extract ADC resolution",
		16, hit250.getADCResolution()
		);
	}
    
    /** @brief Read coarse time from 250 MSPS module. */
    void coarseTime_250 ()
	{
	    EQMSG(
		"250 MSPS compute coarse time",
		uint64_t(0x000a0000f687)*8, hit250.getCoarseTime()
		); 
	}
    
    /** @brief Read computed time from 250 MSPS module. */
    void time_250 ()
	{
	    // The magic number is reconstructed TS for 250 MSPS:
	    ASSERTMSG(
		"250 MSPS compute time",
		std::abs(hit250.getTime() - 343597888565.2810059) < 0.000001
		); 
	}

    /** 
     * @brief Read CFD fail bit from 250 MSPS module. Same bit 31 as 100 MSPS 
     * module but bit 30 contains the trigger source which we'll also check
     * in the next test, so might as well make sure we've read this correctly.
     */
    void cfdFail_250 ()
	{
	    // We can read 0 just as well as 1:
	    EQMSG(
		"250 MSPS compute CFD fail bit",
		uint32_t(0), hit250.getCFDFailBit()
		);
	}
    
    /** @brief Read CFD trigger source from 250 MSPS module. */   
    void cfdTrigSource_250 ()
	{
	    EQMSG(
		"250 MSPS compute CFD trig source bit",
		uint32_t(1), hit250.getCFDTrigSource()
		);
	}

    //_______________________________________________________________________
    // Tests for 500 MSPS modules
    //

    /** @brief Read MSPS from module identifier word for 500 MSPS. */
    void msps_500 ()
	{
	    EQMSG(
		"500 MSPS extract module MSPS",
		uint32_t(500), hit500.getModMSPS()
		); 
	}

    /** @brief Read revision from module idntifier word for 500 MSPS. */
    void revision_500 ()
	{
	    EQMSG(
		"500 MSPS extract hardware revision",
		15, hit500.getHardwareRevision()
		);
	}
    
    /** 
     * @brief Read ADC resolution (bit depth) from module identifier word 
     * for 16-bit 500 MSPS. 
     */
    void resolution_500 ()
	{
	    EQMSG(
		"500 MSPS extract ADC resolution",
		14, hit500.getADCResolution()
		);
	}
    
    /** @brief Read coarse time from 500 MSPS module. */
    void coarseTime_500 ()
	{
	    EQMSG(
		"500 MSPS compute coarse time",
		uint64_t(0x000a0000f687)*10, hit500.getCoarseTime()
		); 
	}
    
    /** @brief Read computed time from 500 MSPS module. */
    void time_500 ()
	{
	    // The magic number is reconstructed TS for 500 MSPS:
	    ASSERTMSG(
		"500 MSPS compute time",
		std::abs(hit500.getTime()-429497360715.281006) < 0.000001
		); 
	}

    /** @brief Read CFD trigger source from 500 MSPS module. */   
    void cfdTrigSource_500 ()
	{
	    EQMSG(
		"500 MSPS compute CFD trig source bit",
		uint32_t(3), hit500.getCFDTrigSource()
		);
	}

    /** 
     * @brief Read CFD fail bit from 500 MSPS module data. Since trigger 
     * source < 7, this must be 0.
     */
    void cfdFail_500_0 ()
	{
	    // Source is < 7 so the fail bit is 0:
	    EQMSG(
		"500 MSPS compute CFD fail bit",
		uint32_t(0), hit500.getCFDFailBit()
		);
	}

    /** 
     * @brief Read CFD fail bit from 500 MSPS module. Check that the fail bit 
     * is properly set when the trigger source == 7.
     */
    void cfdFail_500_1 ()
	{
	    // Bits [31:29] of 0xf47f000a (=111) are the trigger source:
	    vector<uint32_t> data = {
		0x0000000c, 0x0f0e01f4, 0x00084321, 0x0000f687,
		0xf47f000a, 0x000008b3
	    };
    
	    DDASHit hit;
	    DDASHitUnpacker unpacker;
	    unpacker.unpack(data.data(), data.data()+data.size(), hit);
	    
	    // Source is == 7 so the fail bit is 1:
	    EQMSG(
		"500 MSPS compute CFD fail bit",
		uint32_t(1), hit.getCFDFailBit()
		);
	}
    
    //_______________________________________________________________________
    // Tests for external clock
    //
    
    /** 
     * @brief Read in a value for an external clock timestamp when the QDC 
     * sums and energy sums are enabled.
     */
    void externalClock_0()
	{
	    uint64_t expected = 0xc0c0d0d0a0a0b0b;
	    EQMSG(
		"Timestamp extracted with QDC and energy sums",
		expected, hit100.getExternalTimestamp()
		);
	}

    /** 
     * @brief Read in a value for an external clock timestamp when no 
     * additional data is present.
     */
    void externalClock_1()
	{
	    // Header words == 6.
	    std::vector<uint32_t> data = {
		0x0000000e, 0x0c0c0064, 0x000c6000, 0x139f2709,
		0x28170000, 0x00007fff,	0x40302010, 0x00a00a00
	    };
	    
	    DDASHit hit;
	    DDASHitUnpacker unpacker;
	    unpacker.unpack(data.data(), data.data()+data.size(), hit);
    
	    EQMSG(
		"Timestamp extracted when no QDC or energy sums",
		uint64_t(0x00a00a0040302010), hit.getExternalTimestamp()
		);
	}
    
    /** 
     * @brief Read in a value for an external clock timestamp when the QDC 
     * sums are disabled and energy sums are enabled.
     */    
    void externalClock_2()
	{
	    // Header words == 10.
	    std::vector<uint32_t> data = {
		0x0000000e, 0x0c0c0064, 0x0014a000, 0x139f2709,
		0x28170000, 0x00007fff, 0x00000001, 0x00000002, 
		0x00000003, 0x00000004, 0x40302010, 0x00a00a00 };
	    
	    DDASHit hit;
	    DDASHitUnpacker unpacker;
	    unpacker.unpack(data.data(), data.data()+data.size(), hit);
    
	    EQMSG(
		"Timestamp extracted with energy sums and no QDC",
		uint64_t(0x00a00a0040302010), hit.getExternalTimestamp()
		);
	}

    /** 
     * @brief Read in a value for an external clock timestamp when the QDC 
     * sums are enabled and energy sums are disabled.
     */    
    void externalClock_3()
	{
	    // Header words == 14.
	    std::vector<uint32_t> data = {
		0x0000000e, 0x0c0c0064, 0x001ce000, 0x139f2709,
		0x28170000, 0x00007fff, 0x00000001, 0x00000002, 
		0x00000003, 0x00000004, 0x00000005, 0x00000006,
		0x00000007, 0x00000008,	0x40302010, 0x00a00a00
	    };

	    DDASHit hit;
	    DDASHitUnpacker unpacker;
	    unpacker.unpack(data.data(), data.data()+data.size(), hit);
    
	    EQMSG(
		"Timestamp extracted with QDC and no energy sums",
		uint64_t(0x00a00a0040302010), hit.getExternalTimestamp()
		);
	}
    
};

// Register it with the test factory
CPPUNIT_TEST_SUITE_REGISTRATION(UnpackerTests);
