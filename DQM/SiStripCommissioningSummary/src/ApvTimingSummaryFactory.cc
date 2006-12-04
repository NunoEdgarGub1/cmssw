#include "DQM/SiStripCommissioningSummary/interface/ApvTimingSummaryFactory.h"
#include "DQM/SiStripCommissioningSummary/interface/SummaryGenerator.h"
#include "DataFormats/SiStripCommon/interface/SiStripHistoNamingScheme.h"
#include <iostream>
#include <sstream>

using namespace std;

// -----------------------------------------------------------------------------
//
SummaryHistogramFactory<ApvTimingAnalysis>::SummaryHistogramFactory() :
  mon_(sistrip::UNKNOWN_MONITORABLE),
  pres_(sistrip::UNKNOWN_PRESENTATION),
  view_(sistrip::UNKNOWN_VIEW),
  level_(sistrip::root_),
  gran_(sistrip::UNKNOWN_GRAN),
  generator_(0) 
{
} 

// -----------------------------------------------------------------------------
//
SummaryHistogramFactory<ApvTimingAnalysis>::~SummaryHistogramFactory() {
  if ( generator_ ) { delete generator_; }
}

// -----------------------------------------------------------------------------
//
void SummaryHistogramFactory<ApvTimingAnalysis>::init( const sistrip::Monitorable& mon, 
						       const sistrip::Presentation& pres,
						       const sistrip::View& view, 
						       const string& top_level_dir, 
						       const sistrip::Granularity& gran ) {
  
  mon_ = mon;
  pres_ = pres;
  view_ = view;
  level_ = top_level_dir;
  gran_ = gran;
  
  // Retrieve utility class used to generate summary histograms
  if ( generator_ ) { delete generator_; generator_ = 0; }
  generator_ = SummaryGenerator::instance( view );
  
}

//------------------------------------------------------------------------------
//
uint32_t SummaryHistogramFactory<ApvTimingAnalysis>::extract( const map<uint32_t,ApvTimingAnalysis>& data ) {
  
  // Check if data are present
  if ( data.empty() ) { 
    cerr << "[" << __PRETTY_FUNCTION__ << "]" 
	 << " No data in monitorables map!" << endl;
    return 0; 
  }
  
  // Check if instance of generator class exists
  if ( !generator_ ) { 
    cerr << "[" << __PRETTY_FUNCTION__ << "]" 
	 << " NULL pointer to SummaryGenerator object!" << endl;
    return 0;
  }

  // Transfer appropriate monitorables info to generator object
  generator_->clearMap();
  map<uint32_t,ApvTimingAnalysis>::const_iterator iter = data.begin();
  for ( ; iter != data.end(); iter++ ) {
    if ( mon_ == sistrip::APV_TIMING_TIME ) { 
      generator_->fillMap( level_, gran_, iter->first, iter->second.time() ); 
    } else if ( mon_ == sistrip::APV_TIMING_MAX_TIME ) { 
      generator_->fillMap( level_, gran_, iter->first, iter->second.maxTime() ); 
    } else if ( mon_ == sistrip::APV_TIMING_DELAY ) { 
      generator_->fillMap( level_, gran_, iter->first, iter->second.delay() ); 
    } else if ( mon_ == sistrip::APV_TIMING_ERROR ) { 
      generator_->fillMap( level_, gran_, iter->first, iter->second.error() ); 
    } else if ( mon_ == sistrip::APV_TIMING_BASE ) { 
      generator_->fillMap( level_, gran_, iter->first, iter->second.base() ); 
    } else if ( mon_ == sistrip::APV_TIMING_PEAK ) { 
      generator_->fillMap( level_, gran_, iter->first, iter->second.peak() ); 
    } else if ( mon_ == sistrip::APV_TIMING_HEIGHT ) {
      generator_->fillMap( level_, gran_, iter->first, iter->second.height() ); 
    } else { continue; }
  }
  return generator_->size();
}

//------------------------------------------------------------------------------
//
void SummaryHistogramFactory<ApvTimingAnalysis>::fill( TH1& summary_histo ) {

  // Check if instance of generator class exists
  if ( !generator_ ) { 
    cerr << "[" << __PRETTY_FUNCTION__ << "]" 
	 << " NULL pointer to SummaryGenerator object!" << endl;
    return;
  }

  // Check if instance of generator class exists
  if ( !(&summary_histo) ) { 
    cerr << "[" << __PRETTY_FUNCTION__ << "]" 
	 << " NULL pointer to SummaryGenerator object!" << endl;
    return;
  }

  // Check if map is filled
  if ( !generator_->size() ) { 
    cerr << "[" << __PRETTY_FUNCTION__ << "]" 
	 << " No data in the monitorables map!" << endl;
    return; 
  } 

  // Generate appropriate summary histogram 
  if ( pres_ == sistrip::SUMMARY_HISTO ) {
    generator_->summaryHisto( summary_histo );
  } else if ( pres_ == sistrip::SUMMARY_1D ) {
    generator_->summary1D( summary_histo );
  } else if ( pres_ == sistrip::SUMMARY_2D ) {
    generator_->summary2D( summary_histo );
  } else if ( pres_ == sistrip::SUMMARY_PROF ) {
    generator_->summaryProf( summary_histo );
  } else { 
    cerr << "[" << __PRETTY_FUNCTION__ << "]" 
	 << " Unexpected SummaryType value:"
	 << SiStripHistoNamingScheme::presentation( pres_ ) 
	 << endl;
    return; 
  }
  
  // Histogram formatting
  if ( mon_ == sistrip::APV_TIMING_TIME ) {
    generator_->axisLabel( "Timing delay [ns]" );
  } else if ( mon_ == sistrip::APV_TIMING_MAX_TIME ) { 
  } else if ( mon_ == sistrip::APV_TIMING_DELAY ) { 
  } else if ( mon_ == sistrip::APV_TIMING_ERROR ) { 
  } else if ( mon_ == sistrip::APV_TIMING_BASE ) { 
  } else if ( mon_ == sistrip::APV_TIMING_PEAK ) { 
  } else if ( mon_ == sistrip::APV_TIMING_HEIGHT ) {
  } else { 
    cerr << "[" << __PRETTY_FUNCTION__ << "]" 
	 << " Unexpected SummaryHisto value:"
	 << SiStripHistoNamingScheme::monitorable( mon_ ) 
	 << endl;
  } 
  generator_->format( sistrip::APV_TIMING, mon_, pres_, view_, level_, gran_, summary_histo );
  
}

// -----------------------------------------------------------------------------
//
template class SummaryHistogramFactory<ApvTimingAnalysis>;

