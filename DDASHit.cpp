/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2016.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Author:
             Ron Fox
             Jeromy Tompkins
             Aaron Chester
             Facility for Rare Isotope Beams
             Michigan State University
             East Lansing, MI 48824-1321
*/

/**
 * @file DDASHit.cpp
 * @brief Implement DDASHit class used to encapsulate DDAS events.
 */

#include "DDASHit.h"

#include <algorithm>
#include <stdlib.h>

#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "DDASBitMasks.h"

using namespace ddasfmt;

/**
 * @details
 * All member data are zero-initialized.
 */
ddasfmt::DDASHit::DDASHit()
    : m_time(0), m_coarseTime(0), m_externalTimestamp(0), m_energy(0),
      m_timeHigh(0), m_timeLow(0), m_timeCFD(0), m_finishCode(0),
      m_channelLength(0), m_channelHeaderLength(0), m_chanID(0), m_slotID(0),
      m_crateID(0), m_cfdTrigSourceBit(0), m_cfdFailBit(0), m_traceLength(0),
      m_modMSPS(0), m_adcResolution(0), m_hdwrRevision(0),
      m_adcOverflowUnderflow(false), m_energySums(), m_qdcSums(), m_trace() {}

/**
 * @details
 * For primitive types, this sets the values to 0. For vector
 * data (i.e. trace), the vector is cleared and resized to 0.
 */
void ddasfmt::DDASHit::Reset() {
  m_time = 0;
  m_externalTimestamp = 0;
  m_coarseTime = 0;
  m_energy = 0;
  m_timeHigh = 0;
  m_timeLow = 0;
  m_timeCFD = 0;
  m_finishCode = 0;
  m_channelLength = 0;
  m_channelHeaderLength = 0;
  m_chanID = 0;
  m_slotID = 0;
  m_crateID = 0;
  m_cfdTrigSourceBit = 0;
  m_cfdFailBit = 0;
  m_traceLength = 0;
  m_modMSPS = 0;
  m_adcResolution = 0;
  m_hdwrRevision = 0;
  m_adcOverflowUnderflow = false;

  m_energySums.clear();
  m_qdcSums.clear();
  m_trace.clear();
}

void ddasfmt::DDASHit::setTimeHigh(uint32_t datum) {
  m_timeHigh = datum & LOWER_16_BIT_MASK;
}

void ddasfmt::DDASHit::setEnergySums(std::vector<uint32_t> eneSums) {
  if (eneSums.size() != SIZE_OF_ENE_SUMS) {
    std::string msg("Error setting energy sums: Expected ");
    msg += std::to_string(SIZE_OF_ENE_SUMS);
    msg += " 32-bit words but got ";
    msg += std::to_string(eneSums.size());
    throw std::runtime_error(msg);
  }
  m_energySums = std::move(eneSums);
}

void ddasfmt::DDASHit::setQDCSums(std::vector<uint32_t> qdcSums) {
  if (qdcSums.size() != SIZE_OF_QDC_SUMS) {
    std::string msg("Error setting QDC sums: Expected ");
    msg += std::to_string(SIZE_OF_QDC_SUMS);
    msg += " 32-bit words but got ";
    msg += std::to_string(qdcSums.size());
    throw std::runtime_error(msg);
  }
  m_qdcSums = std::move(qdcSums);
}
