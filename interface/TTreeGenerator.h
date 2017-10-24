#include<iostream>

#include "TFile.h"
#include "TTree.h"
#include "TVectorF.h"
#include "TClonesArray.h"

#include "UserCode/DTDPGAnalysis/interface/DefineTreeVariables.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "SimDataFormats/DigiSimLinks/interface/DTDigiSimLinkCollection.h"// To cope with Digis from simulation
#include "DataFormats/Common/interface/DetSetVector.h"

#include "TrackingTools/GeomPropagators/interface/Propagator.h"

#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include <FWCore/Framework/interface/ConsumesCollector.h>
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1Trigger/interface/Muon.h"

class DTTTrigBaseSync;

//
// class declaration
//
class TTreeGenerator : public edm::EDAnalyzer {
  
public:
  explicit TTreeGenerator(const edm::ParameterSet&);
  ~TTreeGenerator() {};
  
  
private:

  virtual void beginJob() ;
  virtual void beginRun(const edm::Run&, const edm::EventSetup&) {};
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&,
                                        edm::EventSetup const&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  DTTTrigBaseSync *theSync;

  void initialize_Tree_variables();
  inline void clear_Arrays();

  void fill_digi_variables(edm::Handle<DTDigiCollection> dtdigis);
  void fill_digi_variablesSim(edm::Handle< DTDigiSimLinkCollection> dtdigisSim);
  void fill_dtsegments_variables(edm::Handle<DTRecSegment4DCollection> segments4D, const DTGeometry* dtGeom_);
  void fill_cscsegments_variables(edm::Handle<CSCSegmentCollection> cscsegments);
  void fill_twinmuxout_variables(edm::Handle<L1MuDTChambPhContainer> localTriggerTwinMuxOut);
  void fill_twinmuxin_variables(edm::Handle<L1MuDTChambPhContainer> localTriggerTwinMuxIn);
  void fill_twinmuxth_variables(edm::Handle<L1MuDTChambThContainer> localTriggerTwinMux_Th);
  void fill_muons_variables(edm::Handle<reco::MuonCollection> MuList);
  void fill_gmt_variables(const edm::Handle<l1t::MuonBxCollection> & gmt);
  void fill_gt_variables(edm::Handle<L1GlobalTriggerReadoutRecord> gtrr, const L1GtTriggerMenu* menu);
  void fill_hlt_variables(const edm::Event& e, edm::Handle<edm::TriggerResults> hltresults);
  void fill_rpc_variables(const edm::Event &e, edm::Handle<RPCRecHitCollection> rpcrechits);
  void Probe_TwinMux_segment(const edm::Event &e, edm::Handle<RPCRecHitCollection> rpcrechits, const auto rpcGeom_); //Reham
  void fill_dtphi_info(const DTChamberRecSegment2D* phiSeg,const GeomDet* geomDet);
  void fill_dtz_info(const DTSLRecSegment2D* zSeg, const GeomDet* geomDet);
  void analyzeBMTF(const edm::Event& e);
  void analyzeRPCunpacking(const edm::Event& e);
  void analyzeUnpackingRpcRecHit(const edm::Event& e);

  TrajectoryStateOnSurface cylExtrapTrkSam(reco::TrackRef track, const float rho) const;
  FreeTrajectoryState freeTrajStateMuon(const reco::TrackRef track) const;

  edm::InputTag dtDigiLabel_;
  edm::EDGetTokenT<DTDigiCollection> dtDigiToken_ ;
  edm::EDGetTokenT<DTDigiSimLinkCollection> dtDigiTokenSim_ ;
  edm::InputTag dtSegmentLabel_;
  edm::EDGetTokenT<DTRecSegment4DCollection> dtSegmentToken_;
  edm::InputTag cscSegmentLabel_;
  edm::EDGetTokenT<CSCSegmentCollection> cscSegmentToken_;
  edm::InputTag dtTrigTwinMuxOutLabel_;
  edm::EDGetTokenT<L1MuDTChambPhContainer> dtTrigTwinMuxOutToken_ ;
  edm::InputTag dtTrigTwinMuxInLabel_;
  edm::InputTag dtTrigTwinMuxThLabel_;
  edm::EDGetTokenT<L1MuDTChambPhContainer> dtTrigTwinMuxInToken_ ;
  edm::EDGetTokenT<L1MuDTChambThContainer> dtTrigTwinMux_ThToken_ ;
  edm::InputTag staMuLabel_;
  edm::EDGetTokenT<reco::MuonCollection> staMuToken_;
  edm::InputTag gmtLabel_;
  edm::EDGetTokenT<l1t::MuonBxCollection> gmtToken_;
  edm::InputTag gtLabel_; // legacy
  edm::EDGetTokenT<L1GlobalTriggerReadoutRecord> gtToken_; // legacy
  edm::InputTag rpcRecHitLabel_;
  edm::EDGetTokenT<RPCRecHitCollection> rpcRecHitToken_;
  edm::InputTag PrimaryVertexTag_;
  edm::EDGetTokenT<reco::VertexCollection> PrimaryVertexToken_ ;
  edm::InputTag beamSpotTag_;
  edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
  edm::InputTag scalersSource_;
  edm::EDGetTokenT<LumiScalersCollection> scalersSourceToken_;
  edm::InputTag triggerTag_;
  edm::EDGetTokenT<edm::TriggerResults> triggerToken_ ;
  edm::InputTag lumiInputTag_;
  edm::EDGetTokenT<LumiDetails> lumiProducerToken_ ;
  edm::EDGetTokenT<LumiSummary> lumiSummaryToken_;
  edm::EDGetTokenT<L1MuDTChambPhContainer> bmtfPhInputTag_;
  edm::EDGetTokenT<L1MuDTChambThContainer> bmtfThInputTag_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> bmtfOutputTag_; 
  edm::EDGetTokenT<MuonDigiCollection<RPCDetId,RPCDigi> > rpcToken_;   
  edm::InputTag UnpackingRpcRecHitLabel_;
  edm::EDGetTokenT<RPCRecHitCollection> UnpackingRpcRecHitToken_;
   
      bool OnlyBarrel_;

  bool runOnRaw_;
  bool runOnSimulation_;
  bool runOnSimulationWithDigis_; // To use when simulation includes also Digis

  bool localDTmuons_;
  bool AnaTrackGlobalMu_;  // To avoid look to the global tracks (The muon collection: vector<reco::Muon> exit,  but not the global tracks:  vector<reco::Track> )
  std::string outFile_;

  edm::ESHandle<MagneticField> theBField;
  edm::ESHandle<Propagator> propagatorAlong;
  edm::ESHandle<Propagator> propagatorOpposite;
  edm::ESHandle<GlobalTrackingGeometry> theTrackingGeometry;

  int digisSize_;
  int dtsegmentsSize_;
  int cscsegmentsSize_;
  int dtltTwinMuxOutSize_;
  int dtltTwinMuxInSize_;
  int dtltTwinMuxThSize_;
  int gmtSize_;
  int STAMuSize_;
  int rpcRecHitSize_;

  //counters
  short idigis;
  short idtsegments;
  short icscsegments;
  short idtltTwinMuxOut;
  short idtltTwinMuxIn;
  short idtltTwinMux_th;
  short imuons;
  short igmt;
  short igtalgo; // legacy
  short igttt; // legacy
  short ihlt;
  short irpcrechits;
  short irpcdigi_TwinMux;
  short irpcrechits_TwinMux;
  short bmtf_size;


  reco::BeamSpot beamspot;

  TFile *outFile;
  TTree *tree_;

};
