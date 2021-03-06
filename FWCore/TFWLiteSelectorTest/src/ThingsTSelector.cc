#include "FWCore/TFWLiteSelectorTest/src/ThingsTSelector.h"
#include <TCanvas.h>
#include <iostream>
#include "Rtypes.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/TestObjects/interface/OtherThingCollection.h"
#include "DataFormats/TestObjects/interface/ThingCollection.h"

using namespace tfwliteselectortest;

static const char* kA = "a";
static const char* kRefA = "refA";
void ThingsTSelector::begin(TList*&)
{
}

void ThingsTSelector::preProcessing(const TList*, TList& out ) {
  if(nullptr != h_a) {
     out.Remove(h_a);
     delete h_a.get();
     h_a = nullptr;
  }
  h_a  = new TH1F( kA , "a"  , 100,  0, 20 );
  out.Add(h_a);

  if(nullptr != h_refA) {
     out.Remove(h_refA);
     delete h_refA.get();
     h_refA = nullptr;
  }
  h_refA  = new TH1F( kRefA , "refA"  , 100,  0, 20 );
  out.Add(h_refA);
}

void ThingsTSelector::process( const edm::Event& iEvent ) {
  std::cout << "processing event " << std::endl;
  //  chain->GetEntry( entry );
  using namespace edmtest;
  edm::Handle<OtherThingCollection> hOThings;

  try {
    iEvent.getByLabel("OtherThing", "testUserTag", hOThings);
    std::cout << ">> other things found:" << hOThings->size() << std::endl;

    for ( size_t i = 0; i < hOThings->size(); ++i ) {
      const OtherThing & thing = (*hOThings)[ i ];
      h_refA ->Fill( thing.ref->a );
      std::cout << ">> ref->a:  " << thing.ref->a << std::endl;
    }

    edm::Handle<ThingCollection> hThings;
    iEvent.getByLabel("Thing",hThings);
    const ThingCollection& things = *hThings;
    std::cout << ">> things found:" << things.size() << std::endl;
    for ( size_t i = 0; i < things.size(); ++i ) {
      const Thing & thing = things[ i ];
      h_a ->Fill( thing.a );
      std::cout << ">> a:  " << thing.a << std::endl;
    }
  } catch (cms::Exception& x) {
    std::cout << std::endl << "Failed with cms::Exception: " << std::endl;
    std::cout << x.what() << std::endl;
    abort();
  } catch (std::exception& x) {
    std::cout << std::endl << "Failed with std::exception" << std::endl;
    std::cout << x.what() << std::endl;
    abort();
  } catch (...) {
    std::cout << std::endl << "Failed with unknown exception" << std::endl;
    abort();
  }

}

void ThingsTSelector::postProcessing(TList&)
{
}

void ThingsTSelector::terminate(TList& out) {
  std::cout << "terminate" << std::endl;
  TCanvas * canvas = new TCanvas( );
  {
     TObject* hist = out.FindObject(kA);
     if(nullptr != hist) {
	hist->Draw();
	canvas->SaveAs( "a.jpg" );
     } else {
	std::cout <<"no '"<<kA<<"' histogram"<< std::endl;
     }
  }
  {
     TObject* hist = out.FindObject(kRefA);
     if(nullptr != hist) {
	hist->Draw();
	canvas->SaveAs( "refA.jpg" );
     } else {
	std::cout <<"no '"<<kRefA<<"' histogram"<< std::endl;
     }
  }
  delete canvas;
}
