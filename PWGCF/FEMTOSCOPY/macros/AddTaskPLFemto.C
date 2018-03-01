
//const Bool_t anaType   = 1;//0 HD; 1 UU;
//const Bool_t OnlineSearchV0 = kFALSE; // True=online, false=offline
//const TString whichV0 = "Lambda"; //Lambda or Kaon
//const TString whichV0region = "signal";//signal or sideband region
//const TString whichMixing = "Tracklets"; //V0M or #Tracklets
//const int whichfilterbit = 96;
//const int whichfilterbit = 128;

AliAnalysisTaskPLFemto *AliAnalysisTaskPLFemto(Int_t system=0/*0=pp,1=PbPb*/,
					       Bool_t theMCon=kFALSE,Bool_t OnlineSearchV0=kFALSE,
                 TString whichV0="Lambda",TString whichV0region="signal", TString trigger="kINT7", bool isRun1=true, int cutVariation = 0)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  if (!mgr) 
    {
      ::Error("AddTaskPLFemtoSpectra", "No analysis manager to connect to.");
      return NULL;
    }  


  //Specify cut values for systematic checks:
  AliFemtoCutValues::systematics DefaultValues = AliFemtoCutValues::kDefault;
  AliFemtoCutValues::systematics ProtonPtDown = AliFemtoCutValues::kProtonVariationLowerPtThresholdDown;
  AliFemtoCutValues::systematics ProtonPtUp = AliFemtoCutValues::kProtonVariationLowerPtThresholdUp;
  AliFemtoCutValues::systematics ProtonEtaUp = AliFemtoCutValues::kProtonVariationEtaRangeUp;
  AliFemtoCutValues::systematics ProtonEtaDown = AliFemtoCutValues::kProtonVariationEtaRangeDown;
  AliFemtoCutValues::systematics ProtonNsigmaUp = AliFemtoCutValues::kProtonVariationNsigmaUp;
  AliFemtoCutValues::systematics ProtonNsigmaDown = AliFemtoCutValues::kProtonVariationNsigmaDown;
  AliFemtoCutValues::systematics ProtonNclsUp = AliFemtoCutValues::kProtonTPCClusterUp;
  AliFemtoCutValues::systematics ProtonFilterBit = AliFemtoCutValues::kProtonVariationFilterBitGlobal;
  AliFemtoCutValues::systematics V0PtDown = AliFemtoCutValues::kV0VariationLowerPtThresholdDown;
  AliFemtoCutValues::systematics V0PtUp = AliFemtoCutValues::kV0VariationLowerPtThresholdUp;
  AliFemtoCutValues::systematics V0CPAUp = AliFemtoCutValues::kV0VariationCosinePointingUp;
  AliFemtoCutValues::systematics V0NsigmaDown  = AliFemtoCutValues::kV0VariationNsigmaDown;
  AliFemtoCutValues::systematics V0NclsUp = AliFemtoCutValues::kV0VariationTPCClusterUp;
  AliFemtoCutValues::systematics V0EtaUp = AliFemtoCutValues::kV0VariationEtaRangeUp;
  AliFemtoCutValues::systematics V0EtaDown = AliFemtoCutValues::kV0VariationEtaRangeDown;
  AliFemtoCutValues::systematics V0DCAdecayDown = AliFemtoCutValues::kV0VariationDCAatV0DecayDown;
  AliFemtoCutValues::systematics V0DCAdaughters = AliFemtoCutValues::kV0VariationDCADaughtersToPVUp;

  printf("CREATE TASK\n");
  // create the task

  //AliAnalysisTaskPLFemto *task[2];

  AliAnalysisTaskPLFemto *task = new AliAnalysisTaskPLFemto("AliAnalysisPLFemto",OnlineSearchV0,whichV0,whichV0region);//calls the constructor of the class
  task->SetRun1(isRun1);
  if(trigger == "kINT7") task->SetTrigger(AliVEvent::kINT7);
  else if(trigger == "kHighMultV0") task->SetTrigger(AliVEvent::kHighMultV0);
  else if(trigger == "kMB") task->SetTrigger(AliVEvent::kMB);
  else {
      std::cout << "Requested trigger type unknown \n";
      return nullptr;
    }
  task->SelectCollisionCandidates(task->GetTrigger());
  if (task->GetTrigger() == AliVEvent::kHighMultV0) task->SetV0Percentile(0.1);

  if(cutVariation == 0 ) task->SetSystematics(DefaultValues);
  if(cutVariation == 1 ) task->SetSystematics(ProtonPtDown);
  if(cutVariation == 2 ) task->SetSystematics(ProtonPtUp);
  if(cutVariation == 3 ) task->SetSystematics(ProtonEtaUp);
  if(cutVariation == 4 ) task->SetSystematics(ProtonEtaDown);
  if(cutVariation == 5 ) task->SetSystematics(ProtonNsigmaUp);
  if(cutVariation == 6 ) task->SetSystematics(ProtonNsigmaDown);
  if(cutVariation == 7 ) task->SetSystematics(ProtonNclsUp);
  if(cutVariation == 8 ) task->SetSystematics(ProtonFilterBit);
  if(cutVariation == 9 ) task->SetSystematics(V0PtDown);
  if(cutVariation == 10 ) task->SetSystematics(V0PtUp);
  if(cutVariation == 11 ) task->SetSystematics(V0CPAUp);
  if(cutVariation == 12 ) task->SetSystematics(V0NsigmaDown);
  if(cutVariation == 13 ) task->SetSystematics(V0NclsUp);
  if(cutVariation == 14 ) task->SetSystematics(V0EtaUp);
  if(cutVariation == 15 ) task->SetSystematics(V0EtaDown);
  if(cutVariation == 16 ) task->SetSystematics(V0DCAdecayDown);
  if(cutVariation == 17 ) task->SetSystematics(V0DCAdaughters);

  task->SetDebugLevel(0);
  task->SetMC(theMCon);

  mgr->AddTask(task);



  // Create and connect containers for input/output

  TString outputfile = AliAnalysisManager::GetCommonFileName();
  outputfile += ":PWGCF_PLFemto";
  //outputfile += usercomment;


  // ------ input data ------
  TString input = "infemto";
  //input += usercomment;
  TString output1 = "Evtinfo";
  //output1 += usercomment;
  TString outputEvt = "listAliEventCuts";
  TString output2 = "SPdir";//directory for single particle quantities
  //output2 += usercomment;
  TString output3 = "PIDdir";//directory for PID quantities
  //output3 += usercomment;
  TString output4 = "TPdir";//directory for two particle quantities
  //output4 += usercomment;

  //AliAnalysisDataContainer *cinput0  = mgr->GetCommonInputContainer();

  AliAnalysisDataContainer *cinput0  =  mgr->CreateContainer(input,TChain::Class(),AliAnalysisManager::kInputContainer);

 // ----- output data -----

  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(output1,TList::Class(),AliAnalysisManager::kOutputContainer,outputfile.Data());
  AliAnalysisDataContainer *coutput2 = mgr->CreateContainer(output2,TList::Class(),AliAnalysisManager::kOutputContainer,outputfile.Data());
  AliAnalysisDataContainer *coutput3 = mgr->CreateContainer(output3,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());
  AliAnalysisDataContainer *coutput4 = mgr->CreateContainer(output4,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());
  AliAnalysisDataContainer *coutput5 = mgr->CreateContainer(outputEvt,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());

  mgr->ConnectInput(task,0,mgr->GetCommonInputContainer());

  mgr->ConnectOutput(task,1,coutput1);
  mgr->ConnectOutput(task,2,coutput2);
  mgr->ConnectOutput(task,3,coutput3);
  mgr->ConnectOutput(task,4,coutput4);
  mgr->ConnectOutput(task,5,coutput5);

  return task;
}



