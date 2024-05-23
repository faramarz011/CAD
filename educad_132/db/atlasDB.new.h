/**
        Atlas DB top level Class
        This Class contains all informations and APIs to use Atlas DB.
        APIs can be used to access all properties and methods.
        properties of the Class are defined in (ATLAS DB APIs document):
		APIs are divided as:
 
			- Library creation interface:		used by library manager / lib file parser
			- Technology creation interface: 	used by technology file parser
			- Technology reading interface: 	used by global and detail router, DRC, extractor, viewer
			- Netlist creation interface: 		used by netlist file parser
			- Netlist reading interface: 		used by viewer, placer
			- Mesh creation and read-through interface: 	used by global router, detail router
			- Updating netlist interface: 		used by : placer
			- File interface ( read/write, text/binary ): 	used by User Interface
 
        ---------------------------------------------------------------
        Updating History:
 
        Non-Implemented Items:
**/

#ifndef ATLAS_DB_H
#define ATLAS_DB_H

#define c_AtlasDB_Version "atlasDB version = 2.58"

#include "atlasDBStructs.h"

class atlasPath ;
class atlasPolygon;
class atlasRect;
class atlasPoint;
class atlasTech;
class atlasLib;
class atlasLibMacro;
class atlasLibPin;
class atlasLibPort;
class atlasDsnNetGeom;
class atlasDsnPin;
class atlasDsnNet;
class atlasTchLayer;
class atlasTchVia;
class atlasDsnDRtLayout;
class atlasDsnGRtMesh;
class atlasDsnBinPlane;
class atlasDsnNetlist;
class atlasDsnInstance;
class atlasDsnProjectStatus;
class atlasDsnDelay;
struct NET_PIN_PORT_INFO;
class ILSpacing;
class CLSpacing;
class atlasTchImplantLayer;
class CLAcCurrentDensity;
class CLDcCurrentDensity;
class MinimumSpacing;
class RLParallel_run_len_entry;
class RLInfluence_entry;
class MinimumCut;
class MinimumEnclosedArea;
class AcCurrentDensity;
class DcCurrentDensity;
class ViaLayerRects;
class atlasTchViaRule;
class SameNetSpacing;
class atlasLibSite;
class LayerGeometry;
class ViaUsed;
class atlasDsnRow;
class atlasDsnVirtualPin;
class atlasDsnBinNet;
class atlasDsnBin;
class atlasDsnVias;
class atlasDsnDie;
class atlasDsnRegion;
class atlasDsnGlobalPin;
class atlasGeneralReport;
class atlasToolReport;

class atlasDB
{
private:
  atlasString BinFilename;
  atlasLib *library;
  atlasTech *technology;
  atlasDsnNetlist * netlist;
  atlasDsnDRtLayout * layout;
  atlasDsnDelay *delays;
  map < atlasString, atlasDsnBinPlane *> binPlanes;
  atlasDsnGRtMesh * mesh;
  atlasDsnProjectStatus *prj_status;
  //atlasDsnDie *mydie;
  atlasDsnRegion *region;
  atlasDsnDie *die;
  atlasGeneralReport *reports;
  


  // pointers keep track of last added objects
  // to fill their list members later
  atlasLibMacro *lastAddedMacro;
  atlasDsnNetGeom *lastAddedObs;
  atlasPath *lastAddedObsPath;
  atlasPolygon *lastAddedObsPolygon;
  atlasDsnPin *lastAddedPin;
  atlasLibPin *lastAddedLibPin;
  atlasPath *lastAddedPathOfPortOfPin;
  atlasPolygon *lastAddedPolygonOfPortOfPin;
  atlasLibPort *lastAddedPort;
  atlasDsnInstance *lastAddedInstance;
  atlasDsnNet *lastAddedNet;
  atlasDsnPin *currentPin;
  atlasPath *lastAddedLayerPath;
  atlasPolygon *lastAddedLayerPolygon;

  // iterators used for reading list and map items
  // and saving current position
  list < atlasRect * >::iterator viaRectListPos;
  list < atlasRect * >::iterator layerRectListPos;
  list < atlasPoint * >::iterator layerPathPointListPos;
  list < atlasPath * >::iterator layerPathListPos;
  list < atlasPoint * >::iterator layerPolygonPointListPos;
  list < atlasPolygon * >::iterator layerPolygonListPos;
  map < atlasString, atlasTchLayer * >::iterator layerListPos;
  map < atlasString, atlasTchVia * >::iterator viaListPos;
  map < atlasString, atlasDsnInstance * >::iterator instListPos;
  map < atlasString, atlasDsnPin * >::iterator instPinListPos;
  map < atlasString, atlasDsnNet * >::iterator instancePinNetListPos;
  map < atlasString, atlasDsnNet * >::iterator netListPos;
  list <atlasDsnPin * >::iterator netPinListPos;
  list <atlasDsnBin *>::iterator netBinListPos;
  list < atlasDsnNet * >::iterator pinNetListPos;
  list < atlasDsnPin * >::iterator pinListPos;
  list < atlasLibPort * >::iterator pinPortListPos;
  list < atlasRect * >::iterator portRectListPos;
  list < atlasPath * >::iterator portPathListPos;
  list < atlasPolygon * >::iterator portPolyListPos;
  list < atlasPoint * >::iterator pathPointListPos;
  list < atlasPoint * >::iterator polyPointListPos;
  list < atlasDsnInstance * >::iterator instListOfBinPos;
  list < atlasDsnNet * >::iterator netListOfBinPos;
  list < atlasLibPort * >::iterator portListOfBinPos;
  list < atlasDsnInstance * >::iterator instListOfActiveVertex;
  list < atlasDsnNet * >::iterator netListOfActiveVertex;
  atlasString activeNetOfActiveVertex;
  list <NET_PIN_PORT_INFO *>::iterator _pinListOfActiveNetOfActiveVertex;
  list <NET_PIN_PORT_INFO *>::iterator _portListOfActiveNetOfActiveVertex;
  list <NET_PIN_PORT_INFO *>::iterator _rectListOfActiveNetOfActiveVertex;
  list <NET_PIN_PORT_INFO *>::iterator _pathListOfActiveNetOfActiveVertex;
  list <atlasRect *>::iterator _rectListOfActivePathOfActiveNetOfActiveVertex;
  list <NET_PIN_PORT_INFO *>::iterator _polygonListOfActiveNetOfActiveVertex;
  list <atlasRect *>::iterator _rectListOfActivePolygonOfActiveNetOfActiveVertex;
  list < atlasDsnPin * >::iterator pinListOfActiveNetOfActiveVertex;
  list < atlasLibPort * >::iterator portListOfActivePinOfActiveVertex;
  list < atlasRect * >::iterator rectListOfActivePortOfActiveVertex;
  list < atlasPath * >::iterator pathListOfActivePortOfActiveVertex;
  list < atlasRect * >::iterator rectListOfpathOfActivePortOfActiveVertex;
  list < atlasPolygon * >::iterator polygonListOfActivePortOfActiveVertex;
  list < atlasRect * >::iterator rectListOfpolygonOfActivePortOfActiveVertex;
  list < TConnectionData * >::iterator connectionListOfActiveVertexPos;
  list < TMyConnectionData * >::iterator myConnectionListOfActiveVertexPos;
  list < atlasRect * >::iterator netRectListPos;
  list < atlasPath * >::iterator netPathListPos;
  list < atlasPoint * >::iterator netPointOfPathListPos;
  list < atlasPolygon * >::iterator netPolygonListPos;
  list < atlasPoint * >::iterator netPointOfPolygonListPos;
  list < atlasRect * >::iterator binRectListPos;
  list < atlasPath * >::iterator binPathListPos;
  list < atlasRect * >::iterator binRectOfPathListPos;
  list < atlasPolygon * >::iterator binPolygonListPos;
  list < atlasRect * >::iterator binRectOfPolygonListPos;
  map < atlasString, atlasLibMacro * >::iterator macroListPos;
  atlasTchVia *lastAddedVia;
  atlasDsnRow *selectedRow;
  list <ILSpacing *>::iterator ImplantLayerSpacingListPos;
  list <CLSpacing *>::iterator cutLayerSpacingListPos;
  list <CLAcCurrentDensity *>::iterator cutLayerAcCurrentDensityListPos;
  list <float>::iterator cutLayerAcCurrentDensityFrequencyListPos;
  list <long int>::iterator cutLayerAcCurrentDensityCutAreaListPos;
  list <float>::iterator cutLayerAcCurrentDensityTableEntriesListPos;
  list <CLDcCurrentDensity *>::iterator cutLayerDcCurrentDensityListPos;
  list <long int>::iterator cutLayerDcCurrentDensityCutAreaListPos;
  list <float>::iterator cutLayerDcCurrentDensityTableEntriesListPos;
  list <MinimumSpacing *>::iterator routingLayerSpacingSpacingListPos;
  list <RLParallel_run_len_entry *>::iterator routingLayerSpacingSpacingTableParallelRunLengthListPos;
  list <RLInfluence_entry *>::iterator routingLayerSpacingSpacingTableInfluenceListPos;
  list <MinimumCut *>::iterator routingLayerMinCutListPos;
  list <MinimumEnclosedArea *>::iterator routingLayerMinEnclosedAreaListPos;
  list <AcCurrentDensity *>::iterator routingLayerAcCurrentDensityListPos;
  list <float>::iterator routingLayerAcCurrentDensityFrequencyListPos;
  list <long int>::iterator routingLayerAcCurrentDensityWidthListPos;
  list <float>::iterator routingLayerAcCurrentDensityTableListPos;
  list <DcCurrentDensity *>::iterator routingLayerDcCurrentDensityListPos;
  list <long int>::iterator routingLayerDcCurrentDensityWidthListPos;
  list <float>::iterator routingLayerDcCurrentDensityTableListPos;
  list <ViaLayerRects *>::iterator viaLayerGeometryListPos;
  list <atlasRect *>::iterator viaLayerGeometryRectListPos;
  map < atlasString, atlasTchViaRule *>::iterator viaRuleListPos;
  list <atlasString>::iterator viaRuleViaNameListPos;
  list <SameNetSpacing *>::iterator sameNetSpacingListPos;
  list <atlasLibSite *>::iterator siteListPos;
  map < atlasString, LayerGeometry * >::iterator macroObstructionListPos;
  map < atlasString, atlasLibPin *>::iterator macroPinListPos;
  list < atlasRect *>::iterator macroObstructionRectListPos;
  list < atlasPath *>::iterator macroObstructionPathListPos;
  list < atlasPoint * >::iterator macroObstructionPathPointsListPos;
  list < atlasPolygon *>::iterator macroObstructionPolygonListPos;
  list < atlasPoint * >::iterator macroObstructionPolygonPointsListPos;
  list < ViaUsed *>::iterator macroObstructionViaListPos;
  list < atlasString >::iterator macroPinLEQListPos;
  list < atlasLibPort * >::iterator macroPinPortListPos;
  map <atlasString, LayerGeometry *>::iterator macroPinPortGeometryListPos;
  list < atlasRect *>::iterator macroPinPortGeometryRectListPos;
  list < atlasPath *>::iterator macroPinPortGeometryPathListPos;
  list < atlasPoint * >::iterator macroPinPortGeometryPathPointsListPos;
  list < atlasPolygon *>::iterator macroPinPortGeometryPolygonListPos;
  list < atlasPoint * >::iterator macroPinPortGeometryPolygonPointsListPos;
  list < ViaUsed *>::iterator macroPinPortGeometryViaListPos;
  list <atlasDsnInstance *>::iterator rowInstanceListPos;
  list <atlasDsnVirtualPin *>::iterator activeVertexVirtualPinListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexNetListPos2;
  list <atlasDsnPin *>::iterator activeVertexPinListPos2;
  list <atlasDsnPin *>::iterator activeVertexNetPinListPos2;
  list < atlasLibPort * >::iterator activeVertexNetPinPortListPos2;
  list < atlasRect * >::iterator activeVertexNetPinPortRectListPos2;
  list <atlasPoint *>::iterator layoutViaListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexOverlapTopNetListPos;
  list <atlasDsnPin *>::iterator activeVertexOverlapTopNetPinListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexOverlapTopLeftNetListPos;
  list <atlasDsnPin *>::iterator activeVertexOverlapTopLeftNetPinListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexOverlapTopRightNetListPos;
  list <atlasDsnPin *>::iterator activeVertexOverlapTopRightNetPinListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexOverlapBottomNetListPos;
  list <atlasDsnPin *>::iterator activeVertexOverlapBottomNetPinListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexOverlapBottomLeftNetListPos;
  list <atlasDsnPin *>::iterator activeVertexOverlapBottomLeftNetPinListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexOverlapBottomRightNetListPos;
  list <atlasDsnPin *>::iterator activeVertexOverlapBottomRightNetPinListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexOverlapLeftNetListPos;
  list <atlasDsnPin *>::iterator activeVertexOverlapLeftNetPinListPos;
  map < atlasString, atlasDsnBinNet *>::iterator activeVertexOverlapRightNetListPos;
  list <atlasDsnPin *>::iterator activeVertexOverlapRightNetPinListPos;
  map <atlasString, atlasDsnRegion *>::iterator floorplanRegionListPos;
  list <atlasPoint *>::iterator floorplanRegionPolygonPointListPos;
  map <atlasString, atlasDsnGlobalPin *>::iterator globalPinListPos;
  map <atlasString, float>::iterator activeVertexAttributeListPos;
  list <atlasDsnVirtualPin *>::iterator floorplanRegionVirtualPinListPos;
  atlasDsnBinPlane *activeBinPlane;
  map < atlasString, float>::iterator binplaneBinAttributeListPos;
  map <string, atlasToolReport *>::iterator toolReportListPos;
public:

  // constructor
  atlasDB();
  ~atlasDB();
  const char *getInfo() {return c_AtlasDB_Version;}

  /**
  ==============================================================================
      Library creation interface
      to be used by : library manager / lib file parser
  ==============================================================================
  **/

  void setManufacturingGrid(long int _grid);
  void setUseMinSpacingForOBS(bool use);
  void setUseMinSpacingForPIN(bool use);
  void setLefSyntaxVersion(float ver);
  void addUnit( TUnitParameters *params );
  void setMaxViaStack( TMaxViaStack *maxVia );
  void setNoWireExtensionAtPin(bool onOff);
  void setSameNetSpacing(TSameNetSpacingParameters *params);
  bool addSite( string name, TSiteParameters *params );
  bool addMacro( string name, TMacroParameters *params  );
  bool setMacroDelay(string name, float delay);
  bool setMacroPinParam(string macroName, string pinName, TPinMacroParameters *params);

  bool Correct_EEQ_LEQ_Integrity();

  /**
  ==============================================================================
      Library reading interface
   
  ==============================================================================
  **/
  bool isManufacturingGridValid();
  bool getManufacturingGrid();
  bool isUseMinSpacingforOBSValid();
  bool getUseMinSpacingForOBS();
  bool isUseMinSpacingForPIN();
  bool getUseMinSpacingForPIN();
  bool isLefSyntaxVersionValid();
  float getLefSyntaxVersion();
  bool isTimeUnitValid();
  TUnitType getTimeUnit();
  long int getTimeConvertFactor();
  bool isCapacitanceUnitValid();
  TUnitType getCapacitanceUnit();
  long int getCapacitanceConvertFactor();
  bool isResistanceUnitValid();
  TUnitType getResistanceUnit();
  long int getResistanceConvertFactor();
  bool isPowerUnitValid();
  TUnitType getPowerUnit();
  long int getPowerConvertFactor();
  bool isCurrentUnitValid();
  TUnitType getCurrentUnit();
  long int getCurrentConvertFactor();
  bool isVoltageUnitValid();
  TUnitType getVoltageUnit();
  long int getVoltageConvertFactor();
  bool isDatabaseUnitValid();
  TUnitType getDatabaseUnit();
  long int getDatabaseConvertFactor();
  bool isFrequencyUnitValid();
  TUnitType getFrequencyUnit();
  long int getFrequencyConvertFactor();
  bool isMaxViaStackValid();
  long int getMaxViaStack();
  bool isMaxViaStackRangeValid();
  string getMaxViaStackRangeFirstLayer();
  string getMaxViaStackRangeSecondLayer();
  bool getNoWireExtensionAtpin();
  long int getSameNetSpacingsNum();
  void resetSameNetSpacingListPos();
  bool hasAnySameNetSpacingInSameNetSpacingList();
  bool sameNetSpacingListGoForward();
  long int getSameNetSpacingMinSpacing();
  bool isSameNetSpacingStackAllowdValid();
  bool getSameNetSpacingStackAllowd();
  string getSameNetSpacingFirstLayerName();
  string getSameNetSpacingSecondLayerName();

  //reading sites
  long int getSiteNum();
  void resetSiteListPos();
  bool hasAnySiteInSiteList();
  bool siteListGoForward();
  string getSiteName();
  TSiteClass getSiteClass();
  bool isSiteSymmetryValid();
  TMacroSymmetry getSiteSymmetry();
  long int getSiteHeight();
  long int getSiteWidth();

  //reading macros
  long int getMacroNum();
  bool setActiveMacroRandomAccess( string macroName);
  void resetMacroListPos();
  bool hasAnyMacroInMacroList();
  bool macroListGoForward();
  string getMacroName();
  TMacroType getMacroType();
  bool isMacroSubTypeValid();
  TMacroSubType getMacroSubType();
  TMacroSource getMacroSource();
  bool isMacroForeignValid();
  string getMacroForeign();
  bool isMacroForeignPointValid();
  long int getMacroForeignPointX();
  long int getMacroForeignPointY();
  bool isMacroForeignOrientValid();
  TOrientation getMacroForeignOrient();
  bool isMacroSiteNameValid();
  string getMacroSiteName();
  bool isMacroSizeValid();
  long int getMacroHeight();
  long int getMacroWidth();
  bool isMacroOriginValid();
  long int getMacroOriginX();
  long int getMacroOriginY();
  TMacroSymmetry getMacroSymmetry();
  bool isMacroEEQValid();
  string getMacroEEQ();
  bool isMacroLEQValid();
  string getMacroLEQ();
  float getMacroDelay();

  //obstructions of macro
  long int getMacroObstructionLayerNum();
  bool setActiveMacroObstructionLayerRandomAccess(string layerName);
  void resetMacroObstructionListPos();
  bool hasAnyObstructionInMacroObstructionList();
  bool macroObstructionListGoForward();
  string getMacroObstructionLayerName();
  bool isMacroObstructionSpacingValid();
  long int getMacroObstructionSpacing();
  bool isMacroObstructionDesignRuleWidthValid();
  long int getMacroObstructionDesignRuleWidth();
  long int getMacroObstructionRectsNum();
  void resetMacroObstructionRectListPos();
  bool hasAnyRectInMacroObstructionRectList();
  bool macroObstructionRectListGoForward();
  long int getMacroObstructionRectTop();
  long int getMacroObstructionRectLeft();
  long int getMacroObstructionRectBottom();
  long int getMacroObstructionRectRight();
  long int getMacroObstructionPathsNum();
  void resetMacroObstructionPathListPos();
  bool hasAnyPathInMacroObstructionPathList();
  bool macroObstructionPathListGoForward();
  TPathEndExt getMacroObstructionPathExt();
  long int getMacroObstructionPathWidth();
  long int getMacroObstructionPathPointsNum();
  void resetMacroObstructionPathPointListPos();
  bool hasAnyPointInMacroObstructionPathPointList();
  bool macroObstructionPathPointListGoForward();
  long int getMacroObstructionPathPointX();
  long int getMacroObstructionPathPointY();
  long int getMacroObstructionPolygonsNum();
  void resetMacroObstructionPolygon();
  bool hasAnyPolygonInMacroObstructionPolygonList();
  bool macroObstructionPolygonListGoForward();
  long int getMacroObstructionPolygonPointsNum();
  void resetMacroObstructionPolygonPointListPos();
  bool hasAnyPointInMacroObstructionPolygonPointList();
  bool macroObstructionPolygonPointListGoForward();
  long int getMacroObstructionPolygonPointX();
  long int getMacroObstructionPolygonPointY();
  long int getMacroObstructionViasNum();
  void resetMacroObstructionViaListPos();
  bool hasAnyViaInMacroObstructionViaList();
  bool macroObstructionViaListGoForward();
  string getMacroObstructionViaName();
  long int getMacroObstructionViaPointX();
  long int getMacroObstructionViaPointY();

  //pins of macro
  long int getMacroPinsNum();
  bool setActiveMacroPinRandomAccess(string pinName);
  void resetMacroPinListPos();
  bool hasAnyPinInMacroPinList();
  bool macroPinListGoForward();
  string getMacroPinName();
  bool isMacroPinTaperRuleValid();
  string getMacroPinTaperRule();
  bool isMacroPinForeignPinNameValid();
  string getMacroPinForeignPinName();
  bool isMacroPinForeignPntValid();
  long int getMacroPinForeignPntX();
  long int getMacroPinForeignPntY();
  bool isMacroPinForeignOrientValid();
  TOrientation getMacroPinForeignOrient();
  bool isMacroPinDirectionValid();
  TPinDirection getMacroPinDirection();
  bool isMacroPinUseValid();
  TPinUse getMacroPinUse();
  bool isMacroPinShapeValid();
  TPinShape getMacroPinShape();
  bool isMacroPinMustJoinValid();
  string getMacroPinMustJoin();
  bool isMacroPinOutNoiseMarginValid();
  float getMacroPinOutNoiseMarginLow();
  float getMacroPinOutNoiseMarginHigh();
  bool isMacroPinOutResistanceValid();
  float getMacroPinOutResistanceLow();
  float getMacroPinOoutResistanceHigh();
  bool isMacroPinInNoiseMarginValid();
  float getMacroPinInNoiseMarginLow();
  float getMacroPinInNoiseMarginHigh();
  bool isMacroPinPowerValid();
  float getMacroPinPower();
  bool isMacroPinLeakageValid();
  float getMacroPinLeakage();
  bool isMacroPinMaxLoadValid();
  float getMacroPinMaxLoad();
  bool isMacroPinCapacitanceValid();
  float getMacroPinCapacitance();
  bool isMacroPinResistanceValid();
  float getMacroPinResistance();
  bool isMacroPinPullDownResValid();
  float getMacroPinPullDownRes();
  bool isMacroPinTieoffrValid();
  float getMacroPinTieoffr();
  bool isMacroPinVHIValid();
  float getMacroPinVHI();
  bool isMacroPinVLOValid();
  float getMacroPinVLO();
  bool isMacroPinRiseVoltageValid();
  float getMacroPinRiseVoltage();
  bool isMacroPinFallVoltageValid();
  float getMacroPinFallVoltage();
  bool isMacroPinRiseThreshValid();
  float getMacroPinRiseThresh();
  bool isMacroPinFallThreshValid();
  float getMacroPinFallThresh();
  bool isMacroPinRiseSatcurValid();
  float getMacroPinRiseSatcur();
  bool isMacroPinFallSatcurValid();
  float getMacroPinFallSatcur();
  bool isMacroPinRiseSlewLimitValid();
  float getMacroPinRiseSlewLimit();
  bool isMacroPinFallSlewLimitValid();
  float getMacroPinFallSlewLimit();
  bool isMacroPinCurrentSourceValid();
  string getMacroPinCurrentSource();
  bool isMacroPinTableNameValid();
  string getMacroPinTableHighName();
  string getMacroPinTableLowName();
  long int getMacroPinLEQsNum();
  void resetMacroPinLEQListPos();
  bool hasAnyLEQInMacroPinLEQList();
  bool macroPinLEQListGoForward();
  string getMacroPinLEQ();

  //ports of pin
  long int getMacroPinPortsNum();
  void resetMacroPinPortListPos();
  bool hasAnyPortInMacroPinPortList();
  bool macroPinPortListGoForward();
  TPortClass getMacroPinPortClass();
  long int getMacroPinPortGeometryNum();
  bool setActiveMacroPinPortGeometryLayerRandomAccess(string layerName);
  void resetMacroPinPortGeometryListPos();
  bool hasAnyGeometryInMacroPinPortGeometryList();
  bool macroPinPortGeometryListGoForward();
  string getMacroPinPortGeometryLayerName();
  bool isMacroPinPortGeometrySpacingValid();
  long int getMacroPinPortGeometrySpacing();
  bool isMacroPinPortGeometryDesignRuleWidthValid();
  long int getMacroPinPortGeometryDesignRuleWidth();
  long int getMacroPinPortGeometryRectsNum();
  void resetMacroPinPortGeometryRectListPos();
  bool hasAnyRectInMacroPinPortGeometryRectList();
  bool macroPinPortGeometryRectListGoForward();
  long int getMacroPinPortGeometryRectTop();
  long int getMacroPinPortGeometryRectLeft();
  long int getMacroPinPortGeometryRectBottom();
  long int getMacroPinPortGeometryRectRight();
  long int getMacroPinPortGeometryPathsNum();
  void resetMacroPinPortGeometryPathListPos();
  bool hasAnyPathInMacroPinPortGeometryPathList();
  bool macroPinPortGeometryPathListGoForward();
  TPathEndExt getMacroPinPortGeometryPathExt();
  long int getMacroPinPortGeometryPathWidth();
  long int getMacroPinPortGeometryPathPointsNum();
  void resetMacroPinPortGeometryPathPointListPos();
  bool hasAnyPointInMacroPinPortGeometryPathPointList();
  bool macroPinPortGeometryPathPointListGoForward();
  long int getMacroPinPortGeometryPathPointX();
  long int getMacroPinPortGeometryPathPointY();
  long int getMacroPinPortGeometryPolygonsNum();
  void resetMacroPinPortGeometryPolygon();
  bool hasAnyPolygonInMacroPinPortGeometryPolygonList();
  bool macroPinPortGeometryPolygonListGoForward();
  long int getMacroPinPortGeometryPolygonPointsNum();
  void resetMacroPinPortGeometryPolygonPointListPos();
  bool hasAnyPointInMacroPinPortGeometryPolygonPointList();
  bool macroPinPortGeometryPolygonPointListGoForward();
  long int getMacroPinPortGeometryPolygonPointX();
  long int getMacroPinPortGeometryPolygonPointY();
  long int getMacroPinPortGeometryViasNum();
  void resetMacroPinPortGeometryViaListPos();
  bool hasAnyViaInMacroPinPortGeometryViaList();
  bool macroPinPortGeometryViaListGoForward();
  string getMacroPinPortGeometryViaName();
  long int getMacroPinPortGeometryViaPointX();
  long int getMacroPinPortGeometryViaPointY();

  /**
  ==============================================================================
      Technology creation interface
      to be used by : technology file parser
  ==============================================================================
  **/

  void addLayer( string name, TLayerType type );
  bool setRoutingLayerParameters(string layerName, TRLparameters *params);
  bool setCutLayerParameters(string layerName, TCLparameters *params);
  bool setImplantLayerParameters(string layerName, TILparameters *params);
  void addVia( string name, TViaParameters *params);
  void addViaRule( string name, TViaRule *params);

  /**
  ==============================================================================
      Technology reading interface
      to be used by : global and detail router, DRC, extractor, viewer
  ==============================================================================
  **/
  bool setActiveLayerRandomAccess(string name);
  long int getLayerNum();//???
  void resetLayerListPos();
  bool hasAnyLayerInLayerList();
  bool layerListPosGoForward();
  string getLayerName();
  TLayerType getLayerType();
  //reading implant layer data
  long int getImplantLayerSpacingNum();//???
  void resetImplantLayerSpacingListPos();
  bool hastAnySpacingInImplantLayerSpacingList();
  bool implantLayerSpacingListPosGoForward();
  long int getImplantLayerSpacingMinSpacing();
  bool isImplantLayerSpacingLayer2nameValid();
  string getImplantLayerSpacingLayer2name();
  bool isImplantLayerWidthValid();
  long int getImplantLayerWidth();

  //reading cut layer data
  long int getCutLayerSpacingNum();//???
  void resetCutLayerSpacingListPos();
  bool hasAnySpacingInCutLayerSpacingList();
  bool cutLayerSpacingListPosGoForward();
  long int getCutLayerSpacingSpacing();
  bool isCutLayerSpacingLayer2nameValid();
  string getCutLayerSpacingLayer2name();
  bool isCutLayerSpacingAdjacentValid();
  long int getCutLayerSpacingAadjacent();
  long int getCutLayerSpacingDistance();
  long int getCutLayerAcCurrentDensityNum();//???
  void resetCutLayerAcCurrentDensityListPos();
  bool hasAnyAcCurrentDensityInCutLayerAcCurrentDensityList();
  bool cutLayerAcCurrentDensityListPosGoForward();
  TcurrentDensityType getCutLayerAcCurrentDensityType();
  bool isCutLayerAcCurrentDensityValueValid();
  float getCutLayerAcCurrentDensityValue();
  bool isCutLayerAcCurrentDensityTableValid();
  long int getCutLayerAcCurrentDensityFrequencyNum();//???
  void resetCutLayerAcCurrentDensityFrequencyListPos();
  bool hasAnyFrequencyInCutLayerAcCurrentDensityFrequencyList();
  bool cutLayerAcCurrentDensityFrequencyListGoForward();
  float getCutLayerAcCurrentDensityFrequencyListValue();
  long int getCutLayerAcCurrentDensityCutAreaNum();//???
  void resetCutLayerAcCurrentDensityCutAreaListPos();
  bool hasAnyCutAreaInCutLayerAcCurrentDensityCutAreaList();
  bool cutLayerAcCurrentDensityCutAreaListGoForward();
  long int getCutLayerAcCurrentDensityCutAreaListValue();
  long int getCutLayerAcCurrentDensityTableNum();//???
  void resetCutLayerAcCurrentDensityTableListPos();
  bool hasAnyValueInCutLayerAcCurrentDensityTableList();
  bool cutLayerAcCurrentDensityTableListGoForward();
  float getCutLayerAcCurrentDensityTableListValue();
  long int getCutLayerDcCurrentDensityNum();//???
  void resetCutLayerDcCurrentDensityListPos();
  bool hasAnyDcCurrentDensityInCutLayerDcCurrentDensityList();
  bool cutLayerDcCurrentDensityListPosGoForward();
  TcurrentDensityType getCutLayerDcCurrentDensityType();
  bool isCutLayerDcCurrentDensityValueValid();
  float getCutLayerDcCurrentDensityValue();
  bool isCutLayerDcCurrentDensityTableValid();
  long int getCutLayerDcCurrentDensityCutAreaNum();//???
  void resetCutLayerDcCurrentDensityCutAreaListPos();
  bool hasAnyCutAreaInCutLayerDcCurrentDensityCutAreaList();
  bool cutLayerDcCurrentDensityCutAreaListGoForward();
  long int getCutLayerDcCurrentDensityCutAreaListValue();
  long int getCutLayerDcCurrentDensityTableNum();//???
  void resetCutLayerDcCurrentDensityTableListPos();
  bool hasAnyValueInCutLayerDcCurrentDensityTableList();
  bool cutLayerDcCurrentDensityTableListGoForward();
  float getCutLayerDcCurrentDensityTableListValue();

  //reading routing layer data
  TRLayerDirection getRoutingLayerDirection();
  long int getRoutingLayerPitch();
  long int getRoutingLayerWidth();
  bool isRoutingLayerOffsetValid();
  long int getRoutingLayerOffset();
  bool isRoutingLayerMinAreaValid();
  long int getRoutingLayerMinArea();
  bool isRoutingLayerSpacingValid();//spacing start
  bool isRoutingLayerSpacingSpacingValid();
  long int getRoutingLayerSpacingSpacingNum();//???
  void resetRoutingLayerSpacingSpacingListPos();
  bool hasAnySpacingInRoutingLayerSpacingSpacingList();
  bool routingLayerSpacingSpacingListGoForward();
  long int getRoutingLayerSpacingSpacingMinSpacing();
  bool isRoutingLayerSpacingSpacingRangeValid();
  long int getRoutingLayerSpacingSpacingRangeMin();
  long int getRoutingLayerSpacingSpacingRangeMax();
  bool isRoutingLayerSpacingSpacingRangeUseLengthThresholdValid();
  bool getRoutingLayerSpacingSpacingRangeInfluenceValid();
  long int getRoutingLayerSpacingSpacingRangeInfluence();
  bool isRoutingLayerSpacingSpacingRangeInfluenceRangeValid();
  long int getRoutingLayerSpacingSpacingRangeInfluenceRangeMin();
  long int getRoutingLayerSpacingSpacingRangeInfluenceRangeMax();
  bool isRoutingLayerSpacingSpacingRangeRangeValid();
  long int getRoutingLayerSpacingSpacingRangeRangeMin();
  long int getRoutingLayerSpacingSpacingRangeRangeMax();
  bool isRoutingLayerSpacingSpacingLengthThresholdValid();
  long int getRoutingLayerSpacingSpacingLengthThreshold();
  bool isRoutingLayerSpacingSpacingLengthThresholdRangeValid();
  long int getRoutingLayerSpacingSpacingLengthThresholdRangeMin();
  long int getRoutingLayerSpacingSpacingLengthThresholdRangeMax();
  bool isRoutingLayerSpacingSpacingTableValid();
  long int getRoutingLayerSpacingSpacingTableParallelRunLengthNum();//???
  void resetRoutingLayerSpacingSpacingTableParallelRunLengthListPos();
  bool hasAnyParallelRunLengthInRoutingLayerSpacingSpacingTableParallelRunLengthList();
  bool routingLayerSpacingSpacingTableParallelRunLengthListGoForward();
  long int getRoutingLayerSpacingSpacingTableParallelRunLengthLength();
  long int getRoutingLayerSpacingSpacingTableParallelRunLengthWidth();
  long int getRoutingLayerSpacingSpacingTableParallelRunLengthSpacing();
  bool isRoutingLayerSpacingSpacingTableInfluenceValid();
  long int getRoutingLayerSpacingSpacingTableInfluenceNum();//???
  void resetRoutingLayerSpacingSpacingTableInfluenceListPos();
  bool hasAnyInfluenceInRoutingLayerSpacingSpacingTableInfluenceList();
  bool routingLayerSpacingSpacingTableInfluenceGoForward();
  long int getRoutingLayerSpacingSpacingTableInfluenceWidth();
  long int getRoutingLayerSpacingSpacingTableInfluenceDistance();
  long int getRoutingLayerSpacingSpacingTableInfluenceSpacing();
  bool isRoutingLayerWireExtensionValid();
  long int getRoutingLayerWireExtension();
  long int getRoutingLayerMinCutNum();//???
  void resetRoutingLayerMinCutListPos();
  bool hasAnyMinCutInRoutingLayerMinCutList();
  bool routingLayerMinCutListGoForward();
  long int getRoutingLayerMinCutMinCut();
  long int getRoutingLayerMinCutWidth();
  TFromSide getRoutingLayerMinCutFromSide();
  bool isRoutingLayerMinCutLengthValid();
  long int getRoutingLayerMinCutLength();
  bool isRoutingLayerMinCutDistanceValid();
  long int getRoutingLayerMinCutDistance();
  bool isRoutingLayerMaxWidthValid();
  long int getRoutingLayerMaxWidth();
  bool isRoutingLayerMinWidthValid();
  long int getRoutingLayerMinWidth();
  long int getRoutingLayerMinEnclosedAreaNum();//???
  void resetRoutingLayerMinEnclosedAreaListPos();
  bool hasAnyMinEnclosedAreaInRoutingLayerMinEnclosedAreaList();
  bool routingLayerMinEnclosedAreaListGoForward();
  long int getRoutingLayerMinEnclosedAreaArea();
  bool isRoutingLayerMinEnclosedAreaWidthValid();
  long int getRoutingLayerMinEnclosedAreaWidth();
  bool isRoutingLayerProtrusionValid();
  long int getRoutingLayerProtrusionWidth1();
  long int getRoutingLayerProtrusionLength();
  long int getRoutingLayerProtrusionWidth2();
  bool isRoutingLayerResistanceValid();
  float getRoutingLayerResistance();
  bool isRoutingLayerCapacitanceValid();
  float getRoutingLayerCapacitance();
  bool isRoutingLayerHeightValid();
  long int getRoutingLayerHeight();
  bool isRoutingLayerThicknessValid();
  long int getRoutingLayerThickness();
  bool isRoutingLayerShrinkageValid();
  long int getRoutingLayerShrinkage();
  bool isRoutingLayerCapMultiplierValid();
  long int getRoutingLayerCapMultiplier();
  bool isRoutingLayerEdgeCapacitanceValid();
  float getRoutingLayerEdgeCapacitance();
  bool isRoutingLayerSlotMinWireWidthValid();
  long int getRoutingLayerSlotMinWireWidth();
  bool isRoutingLayerSlotMinWireLengthValid();
  long int getRoutingLayerSlotMinWireLength();
  bool isRoutingLayerSlotMinWidthValid();
  long int getRoutingLayerSlotMinWidth();
  bool isRoutingLayerSlotMinLengthValid();
  long int getRoutingLayerSlotMinLength();
  bool isRoutingLayerMaxAdjSlotSpacingValid();
  long int getRoutingLayerMaxAdjSlotSpacing();
  bool isRoutingLayerMaxCoaxialSlotSpacingValid();
  long int getRoutingLayerMaxCoaxialSlotSpacing();
  bool isRoutingLayerMaxEdgeSlotSpacingValid();
  long int getRoutingLayerMaxEdgeSlotSpacing();
  bool isRoutingLayerSplitWireWidthValid();
  long int getRoutingLayerSplitWireWidth();
  bool isRoutingLayerMinDencityValid();
  long int getRoutingLayerMinDencity();
  bool isRoutingLayerMaxDencityValid();
  long int getRoutingLayerMaxDencity();
  bool isRoutingLayerDensityCheckWindowValid();
  long int getRoutingLayerDensityCheckWindowLength();
  long int getRoutingLayerDensityCheckWindowWidth();
  bool isRoutingLayerDensityCheckStepValid();
  long int getRoutingLayerDensityCheckStep();
  bool isRoutingLayerMinStepValid();
  long int getRoutingLayerMinStep();
  bool isRoutingLayerFillActiveSpacingValid();
  long int getRoutingLayerFillActiveSpacing();
  long int getRoutingLayerAcCurrentDensityNum();//???
  void resetRoutingLayerAcCurrentDensityListPos();
  bool hasAnyAcCurrentInRoutingLayerAcCurrentDensityList();
  bool routingLayerAcCurrentDensityListGoForward();
  TcurrentDensityType getRoutingLayerAcCurrentDensityType();
  bool isRoutingLayerAcCurrentDensityValueValid();
  float getRoutingLayerAcCurrentDensityValue();
  bool isRoutingLayerAcCurrentDensityTableValid();
  long int getRoutingLayerAcCurrentDensityFrequencyNum();//???
  void resetRoutingLayerAcCurrentDensityFrequencyListPos();
  bool hasAnyFrequencyInRoutingLayerAcCurrentDensityFrequencyList();
  bool routingLayerAcCurrentDensityFrequencyListGoForward();
  float getRoutingLayerAcCurrentDensityFrequencyListValue();
  long int getRoutingLayerAcCurrentDensityWidthNum();//???
  void resetRoutingLayerAcCurrentDensityWidthListPos();
  bool hasAnyWidthInRoutingLayerAcCurrentDensityWidthList();
  bool routingLayerAcCurrentDensityWidthListGoForward();
  long int getRoutingLayerAcCurrentDensityWidthListValue();
  long int getRoutingLayerAcCurrentDensityTableNum();//???
  void resetRoutingLayerAcCurrentDensityTableListPos();
  bool hasAnyValueInRoutingLayerAcCurrentDensityTableList();
  bool routingLayerAcCurrentDensityTableListGoForward();
  float getRoutingLayerAcCurrentDensityTableListValue();
  long int getRoutingLayerDcCurrentDensityNum();//???
  void resetRoutingLayerDcCurrentDensityListPos();
  bool hasAnyDcCurrentInRoutinLayerDcCurrentDensityList();
  bool routingLayerDcCurrentDensityListGoForward();
  TcurrentDensityType getRoutingLayerDcCurrentDensityType();
  bool isRoutingLayerDcCurrentDensityValueValid();
  float getRoutingLayerDcCurrentDensityValue();
  bool isRoutingLayerDcCurrentDensityTableValid();
  long int getRoutingLayerDcCurrentDensityWidthNum();//???
  void resetRoutingLayerDcCurrentDensityWidthListPos();
  bool hasAnyWidthInRoutingLayerDcCurrentDensityWidthList();
  bool routingLayerDcCurrentDensityWidthListGoForward();
  long int getRoutingLayerDcCurrentDensityWidthListValue();
  long int getRoutingLayerDcCurrentDensityTableNum();//???
  void resetRoutingLayerDcCurrentDensityTableListPos();
  bool hasAnyValueInRoutingLayerDcCurrentDensityTableList();
  bool routingLayerDcCurrentDensityTableListGoForward();
  float getRoutingLayerDcCurrentDensityTableListValue();

  //reading via data
  bool setActiveViaRandomAccess(string name);
  long int getViaNum();//???
  void resetViaListPos();
  bool hasAnyViaInViaList();
  bool viaListGoForward();
  string getViaName();
  bool getViaDefaultVia();
  bool getViaTopOfStackOnly();
  bool isViaForeignCellNameValid();
  string getViaForeignCellName();
  bool isViaForeignPointValid();
  long int getViaForeignPointX();
  long int getViaForeignPointY();
  bool isViaForeignOrientValid();
  TOrientation getViaForeignOrient();
  bool isViaResistanceValid();
  float getViaResistance();
  long int getViaLayerGeometryNum();//???
  void resetViaLayerGeometryListPos();
  bool hasAnyGeometryInViaLayerGeometryList();
  bool viaLayerGeometryListGoForward();
  string getViaLayerGeometryLayerName();
  long int getViaLayerGeometryRectNum();//???
  void resetViaLayerGeometryRectListPos();
  bool hasAnyRectInViaLayerGeometryRectList();
  bool viaLayerGeometryRectListGoForward();
  long int getViaLayerGeometryRectListValueTop();
  long int getViaLayerGeometryRectListValueLeft();
  long int getViaLayerGeometryRectListValueBottom();
  long int getViaLayerGeometryRectListValueRight();

  //reading via rule
  bool setActiveViaRuleRandomAccess(string name);
  long int getViaRuleNum();//???
  void resetViaRuleLisPos();
  bool hasAnyViaRuleInViaRuleList();
  bool viaRuleListGoForward();
  string getViaRuleName();
  string getViaRuleLayerName1();
  TRLayerDirection getViaRuleLayerDirection1();
  bool isViaRuleWidth1Valid();
  long int getViaRuleMinWidth1();
  long int getViaRuleMaxWidth1();
  bool isViaRuleOverhang1Valid();
  long int getViaRuleOverhang1();
  bool isViaRuleMetalOverhang1Valid();
  long int getViaRuleMetalOverhang1();
  bool isViaRuleEnclosure1Valid();
  long int getViaRuleEnclosureOverhang1_1();
  long int getViaRuleEnclosureOverhang2_1();
  string getViaRuleLayerName2();
  TRLayerDirection getViaRuleLayerDirection2();
  bool isViaRuleWidth2Valid();
  long int getViaRuleMinWidth2();
  long int getViaRuleMaxWidth2();
  bool isViaRuleOverhang2Valid();
  long int getViaRuleOverhang2();
  bool isViaRuleMetalOverhang2Valid();
  long int getViaRuleMetalOverhang2();
  bool isViaRuleEnclosure2Valid();
  long int getViaRuleEnclosureOverhang1_2();
  long int getViaRuleEnclosureOverhang2_2();
  long int getViaRuleViaNameNum();
  void resetViaRuleViaNamelistPos();
  bool hasAnyViaNameInViaRuleViaNameList();
  bool viaRuleViaNameListGoForward();
  string viaRuleViaNameListValue();
  bool isViaRuleGenerate();
  string getViaRuleCutLayerName();
  long int getViaRuleCutRectTop();
  long int getViaRuleCutRectLeft();
  long int getViaRuleCutRectBottom();
  long int getViaRuleCutRectRight();
  long int getViaRuleCutXSpacing();
  long int getViaRuleCutYSpacing();
  bool isViaRuleCutResistanceValid();
  float getViaRuleCutResistance();

  /**
  ==============================================================================
      Netlist creation interface
      to be used by : netlist file parser
  ==============================================================================
  **/

  bool addPad(string name, string macro_name);
  bool addPinToPad(string name);
  bool addInstance( string name, string macroName);
  void setInstancePlaceType( TInstPlaceType _placeType, TPointData _pnt, TOrientation _orient);
  void setInstanceWeight( long int _weight);
  void setInstanceRegion( string _regionName);
  bool addPinToInstance( string name );//????????????????????
  bool copyLibPinsToInst();
  bool addNet( string name );
  bool addSpecialNet(string name);
  void setNetEstCap(float _est);
  void setNetWeight(float _weight);
  void setNetSource(TNetSource _source);
  void setNetPattern(TNetPattern _pattern);
  void setNetUse(TNetUse _use);
  bool connectNetToPin( string pinName, string instName);
  bool connectNetToGlobalPin(string globalPinName);
  //create placement rows
  void setPlacementRows(long int rowsNumber);
  bool setPlacementRowWidth(long int rowID, long int width);
  bool setPlacementRowHeight(long int rowID, long int height);
  bool setPlacementRowTop(long int rowID, long int top);
  bool setPlacementRowLeft(long int rowID, long int left);
  //add instance to placement row
  bool addInstanceToRow(long int rowID, string instName);
  //remove instance from placement row
  bool removeInstanceFromRow(long int rowID, string instName);
  //gloab pins
  bool addGlobalPin(string pinName);
  bool setGlobalPinParameters(string pinName, TGlobalPinParameters *params);

  /**
  ==============================================================================
      Updating netlist interface, placement results
      to be used by : placer
  ==============================================================================
  **/

  // Instance writng interface - placer
  //bool setInstanceBottomLeft( string instanceName, long int bottom, long int left );
  bool setInstanceTopLeft(string instanceName, long int top, long int left, TOrientation orient, TInstPlaceType placeType);
  bool setInstanceRegion(string instanceName, string regionName);
  // Placement results writing interface - placer
  void setNetlistPlacementResults( long int Width, long int Height, long int Rownumber, long int Rowheight);

  bool setNetWeight(string netName, float weight);//used by net weighting
  /**
  ==============================================================================
      Netlist reading interface
      to be used by : viewer, placer
  ==============================================================================
  **/
  //reading placement results
  long int getPlacementResultWidth();
  long int getPlacementResultHeight();
  long int getPlacemnetResultRowNumber();
  long int getPlacementResultRowHeight();
  bool getPlacementResultPlaced();

  //Instances reading
  long int getInstanceNum();
  bool setActiveInstanceRandomAccess(string instanceName);
  bool setActiveInstanceRandomAccess(long int x, long int y);
  void resetInstanceListPos();
  bool hasAnyInstanceInInstanceList();
  bool InstanceListPosGoForward();
  string getInstanceName();
  string getInstanceMacroName();
  //long int getInstanceBottom();
  long int getInstanceTop();
  long int getInstanceLeft();
  long int getInstanceWeight();
  string getInstanceRegion();
  TInstPlaceType getInstancePlaceType();
  long int getInstancePlacePointX();
  long int getInstancePlacePointY();
  TOrientation getInstancePlaceOrient();

  //instance->pins reading
  long int getInstancePinNum();
  bool setActiveInstancePinRandomAccess(string instanceName, string pinName);
  bool setActiveInstancePinRandomAccess(string pinName);
  void resetInstancePinListPos();
  bool hasAnyPinInInstancePinList();
  bool instancePinListPosGoForward();
  string getInstancePinName();
  string getInstancePinNetName();

  //special net reading
  long int getSpecialNetNum();
  bool setActiveSpecialNetRandomAccess(string netName);
  void resetSpecialNetListPos();
  bool hasAnySpecialNetInSpecialNetList();
  bool specialNetListPosGoForward();

  // nets reading
  long int getNetNum();
  bool setActiveNetRandomAccess(string netName);
  bool setActiveNetRandomAccess(string layerName, long int x, long int y);
  void resetNetListPos();
  bool hasAnyNetInNetList();
  bool netListPosGoForward();
  string getNetName();
  TNetUse getNetUse();
  float getNetWeight();
  TNetPattern getNetPattern();
  TNetSource getNetSource();
  float getNetEstimatedCapacitance();
  bool isNetGlobalPinNameValid();
  string getNetGlobalPinName();

  //net->pins reading
  long int getNetPinNum();
  bool setActiveNetPinRandomAccess(string netName, string instName, string pinName);
  bool setActiveNetPinRandomAccess(string instName, string pinName);
  void resetNetPinListPos();
  bool hasAnyPinInNetPinList();
  bool netPinListPosGoForward();
  string getNetPinName();
  string getNetPinInstanceName();

  //net->bins reading
  long int getNetBinNum();
  bool resetNetBinListPos();
  bool hasAnyBinInNetBinList();
  bool netBinListPosGoForward();
  long int getNetBinRow();
  long int getNetBinColumn();

  //reading placement rows
  long int getPlacementRowNumber();
  bool setActivePlacementRow(long int rowID);
  long int getPlacementRowWidth();
  long int getPlacementRowHeight();
  long int getPlacementRowTop();
  long int getPlacementRowLeft();
  long int getPlacementRowInstancesNum();
  void resetPlacementRowInstances();
  bool hasAnyInstanceInPlacementRow();
  bool placementRowInstanceGoForward();
  string getPlacementRowInstanceName();

  //global pins
  long int getGlobalPinNum();
  bool setActiveGlobalPinRandomAccess(string gPinName);
  void resetGlobalPinListPos();
  bool hasAnyGlobalPinInGlobalPinList();
  bool globalPinListPosGoForward();
  string getGlobalPinName();
  string getGlobalPinNetName();
  TPinDirection getGlobalPinDirection();
  TPinUse getGlobalPinUse();
  bool getGlobalPinSpecial();
  bool isGlobalPinGeomValid();
  string getGlobalPinGeomLayerName();
  long int getGlobalPinGeomTop();
  long int getGlobalPinGeomLeft();
  long int getGlobalPinGeomBottom();
  long int getGlobalPinGeomRight();
  TGlobalPinPlaceType getGlobalPinPlaceType();
  long int getGlobalPinPlaceX();
  long int getGlobalPinPlaceY();
  TOrientation getGlobalPinPlaceOrientation();
  /**
  ==============================================================================
      floorplan creation: 
  ==============================================================================
  **/

  bool setFloorplanDieNetlist();
  void setFloorplanDieWidth(long int width);
  void setFloorplanDieHeight(long int height);
  void setFloorplanDieAspectRatio(float aspectRatio);
  void setFloorplanDieWhiteSpacePercentage(float whiteSpacePercentage);
  void removeFloorplanDieRegions();
  void addFloorplanDieRegion(string regionName, TRegionType type);
  void setFloorplanDieRegionPolygon(string regionName, TPolygonData *polygon);
  void addFloorplanDieRegionVirtualPin(string regionName, string netName, string layerName, long int x, long int y);
  bool setFloorplanDieRegionPlacmentOffset(string regionName, long int x, long int y);

  /**
  ==============================================================================
      floorplan reading: 
  ==============================================================================
  **/

  bool getFloorplanDieNetlist();
  long int getFloorplanDieWidth();
  long int getFloorplanDieHeight();
  float getFloorplanDieAspectRatio();
  float getFloorplanDieWhiteSpacePercentage();
  long int getFloorplanDieRegionNum();
  bool setActiveFloorplanDieRegion(string regionName);
  void resetFloorplanDieRegionListPos();
  bool hasAnyRegionInFloorplanDieRegionList();
  bool floorplanDieRegionListPosGoForward();
  string getFloorplanDieRegionName();
  TRegionType getFloorplanDieRegionType();
  long int getFloorplanDieRegionPlacementXOffset();
  long int getFloorplanDieRegionPlacementYOffset();
  long int getFoolplanDieRegionBoundingRectTop();
  long int getFoolplanDieRegionBoundingRectLeft();
  long int getFoolplanDieRegionBoundingRectBottom();
  long int getFoolplanDieRegionBoundingRectRight();
  long int getFloorplanDieRegionPolygonPointNum();
  void resetFloorplanDieRegionPolygonPointListPos();
  bool hasAnyPointInFloorplanDieRegionPolygonPointList();
  bool floorplanDieRegionPolygonPointListPosGoForward();
  long int getFloorplanDieRegionPolygonPointX();
  long int getFloorplanDieRegionPolygonPointY();
  long int getFloorplanDieRegionVirtualPinNum();
  bool resetFloorplanDieRegionVirtualPinListPos();
  bool hasAnyVirtualPinInFloorplanDieRegionVirtualPinList();
  bool floorplanDieRegionVirtualPinListPosGoForward();
  string getFloorplanDieRegionVirtualPinNetName();
  string getFloorplanDieRegionVirtualPinLayerName();
  long int getFloorplanDieRegionVirtualPinX();
  long int getFloorplanDieRegionVirtualPinY();


  /**
  ==============================================================================
      Layout creation interface
      to be used by : detail router
  ==============================================================================
  **/
  //add rect to layer
  bool addRectToLayer( string layerName, long int top, long int left, long int bottom, long int right );
  bool addRectToLayer( string layerName, long int top, long int left, long int bottom, long int right , string net);
  //add path to layer
  bool addPathToLayer( string layerName, long int width, TPathEndExt ext );
  bool addPathToLayer( string layerName, long int width, TPathEndExt ext, string net );
  bool addPointToLayerPath( long int x, long int y, bool lastPoint );
  //add polygon to layer
  bool addPolygonToLayer( string layerName );
  bool addPolygonToLayer( string layerName, string net );
  bool addPointToLayerPolygon( long int x, long int y, bool lastPoint );
  //add via to layer
  bool addLayoutVia(string viaName, long int x, long int y);

  /**
  ==============================================================================
      Layout reading interface
      to be used by : layout generator, design rule checker, circuit extractor, ...
  ==============================================================================
  **/
  // Layers
  void setActiveLayoutLayerRandomAccess( string layerName);
  //reading rects of active layer
  long int getLayoutLayerRectNum();
  void resetLayoutLayerRectListPos();
  bool hasAnyRectInLayoutLayerRectList();
  bool layoutLayerRectListPosGoForward();
  string getLayoutLayerRectNetName();
  long int getLayoutLayerRectTop();
  long int getLayoutLayerRectLeft();
  long int getLayoutLayerRectBottom();
  long int getLayoutLayerRectRight();
  //reading paths of active layer
  long int getLayoutLayerPathNum();
  void resetLayoutLayerPathListPos();
  bool hasAnyPathInLayoutLayerPathList();
  bool layoutLayerPathListPosGoForward();
  TPathEndExt getLayoutLayerPathEndExtension();
  long int getLayoutLayerPathWidth();
  string getLayoutLayerPathNetName();
  long int getLayoutLayerPathPointNum();
  void resetLayoutLayerPathPointListPos();
  bool hasAnyPointInLayoutLayerPathPointList();
  bool layoutLayerPathPointListPosGoForward();
  long int getLayoutLayerPathPointX();
  long int getLayoutLayerPathPointY();
  //reading polygons of active layer
  long int getLayoutLayerPolygonNum();
  void resetLayoutLayerPolygonListPos();
  bool hasAnyPolygonInLayoutLayerPolygonList();
  bool layoutLayerPolygonListPosGoForward();
  string getLayoutLayerPolygonNetName();
  long int getLayoutLayerPolygonPointNum();
  void resetLayoutLayerPolygonPointListPos();
  bool hasAnyPointInLayoutLayerPolygonPointList();
  bool layoutLayerPolygonPointListPosGoForward();
  long int getLayoutLayerPolygonPointX();
  long int getLayoutLayerPolygonPointY();

  //reading rects of a net
  long int getLayoutNetRectNum(string netName);
  bool resetLayoutNetRectListPos(string netName);
  bool hasAnyRectInLayoutNetRectList(string netName);
  bool layoutNetRectListPosGoForward(string netName);
  long int getLayoutNetRectTop();
  long int getLayoutNetRectLeft();
  long int getLayoutNetRectBottom();
  long int getLayoutNetRectRight();
  //reading paths of a net
  long int getLayoutNetPathNum(string netName);
  bool resetLayoutNetPathListPos(string netName);
  bool hasAnyPathInLayoutNetPathListPos(string netName);
  bool layoutNetPathListPosGoForward(string netName);
  string getLayoutNetPathLayerName();
  TPathEndExt getLayoutNetPathEndExtension();
  long int getLayoutNetPathWidth();
  long int getLayoutNetPathPointNum();
  void resetLayoutNetPathPointListPos();
  bool hasAnyPointInLayoutNetPathPointList();
  bool layoutNetPathPointListPosGoForward();
  long int getLayoutNetPathPointX();
  long int getLayoutNetPathPointY();
  //reading polygons of a net
  long int getLayoutNetPolygonNum(string netName);
  bool resetLayoutNetPolygonListPos(string netName);
  bool hasAnyPolygonInLayoutNetPolygonList(string netName);
  bool layoutNetPolygonListPosGoForward(string netName);
  long int getLayoutNetPolygonPointNum();
  void resetLayoutNetPolygonPointListPos();
  bool hasAnyPointInLayoutNetPolygonPointList();
  bool layoutNetPolygonPointListPosGoForward();
  long int getLayoutNetPolygonPointX();
  long int getLayoutNetPolygonPointY();

  //reading layout->vias
  bool setLayoutViaName(string viaName);
  long int getLayoutViaNum();
  void resetLayoutViaListPos();
  bool hasAnyViaInLayoutViaList();
  bool layoutViaListPosGoForward();
  string getLayoutViaName();
  long int getLayoutViaX();
  long int getLayoutViaY();

  /**
  ==============================================================================
      Delay interfaces
  	instance delay
  	net delay
   
  ==============================================================================
  **/
  void addDelayToInstance( string instName, float delay );
  void addDelayToNet( string netName, string pin1Name, string pin2Name, float delay );
  void addDelayToPath( string sourceInst, string sourcePin, string destInst, string destPin, float delay );
  float getInstanceDelay( string instName);
  float getNetDelay( string netName, string pin1Name, string pin2Name );
  float getPathDelay( string sourceInst, string sourcePin, string destInst, string destPin);

  /**
  ==============================================================================
      Bin creation and read-through interface
      to be used by : viewer,
  ==============================================================================
  **/

  bool createBinPlane( string bpName, string regionName, long int verticalPitchSize, long int horizontalPitchSize, long int verticalTrackNumber, long int horizontalTrackNumber, long int _verticalOverlapPitch, long int _horizontalOverlapPitch);
  long int getBinPlaneRowNum( string bpName );
  long int getBinPlaneColNum( string bpName );
  bool assignInstancesToBins( string bpName );
  bool assignNetsAndPortsToBins( string bpName );
  bool assignObstructionsToLayout();
  void destroyBinPlane( string bpName );
  bool setActiveBinPlane(string bpName);
  bool setActiveBin(long int row, long int column );
  bool setActiveBinAttribute(string attributeName, float attributeValue);
  bool resetActiveBinAttributeListPos();
  bool hasAnyAttributeInActiveBinAttributeList();
  bool activeBinAttributeListPosGoForward();
  string getActiveBinAttributeName();
  float getActiveBinAttributeValue();


  /**
  ==============================================================================
      Mesh creation and read-through interface
      to be used by : global router, detail router
  ==============================================================================
  **/

  bool createMesh( string regionName, long int verticalPitchSize, long int horizontalPitchSize, long int verticalTrackNumber, long int horizontalTrackNumber, long int _verticalOverlapPitch, long int _horizontalOverlapPitch);
  long int getMeshRowNum();
  long int getMeshColNum();
  long int getVertexHeight();
  long int getVertexWidth();
  long int getNumberOfVerticalPitchOfVertex();
  long int getNumberOfHorizontalPitchOfVertex();
  bool assignInstancesToMesh();
  bool assignNetsAndPortsToMesh();

  void setActiveVertex( long int row, long int column );
  void printActiveVertexDataToFile(long int row, long int column, bool pfInstance, bool pfVirtualPin, bool pfNet, bool pfPin, bool pPort);
  void printActiveVertexDataToFile(long int row, long int column, bool pfInstance, bool pfVirtualPin, bool pfNet, bool pfPin, bool pfPort, bool pfPortRect, bool pfPortPath, bool pfPortPolygon, bool pfRect, bool pfPath, bool pfPolygon);

  bool setActiveVertexAttribute(long int row, long int column, string attributeName, float attributeValue);
  void resetActiveVertexAttributeListPos();
  bool hasAnyAttributeInActiveVertexAttributeList();
  bool activeVertexAttributeListPosGoForward();
  string getActiveVertexAttributeName();
  float getActiveVertexAttributeValue();

  //global router add connection to edge of selected vertex
  void addConnectionToActiveVertex(string netName, TEdge edge ,string layerName);
  //detail router add connection to edge of selected vertex and assigned to each track a number
  void addConnectionToActiveVertex(string netName, TEdge edge, string layerName, long int netAssignedTrack);
  //detail router or global router can reset list of any edge of selected vertex
  void clearConnectionsFromActiveVertex(TEdge edge);
  void clearConnectionsFromActiveVertex(TEdge edge, string layerName);
  //get top connection data of selected vertex
  void resetTopMyConnectionOfActiveVertexPos();
  bool getTopMyConnectionOfActiveVertexGoForward( TMyConnectionData * );
  //get left connection data of selected vertex
  void resetLeftMyConnectionOfActiveVertexPos();
  bool getLeftMyConnectionOfActiveVertexGoForward( TMyConnectionData * );
  //get bottom connection data of selected vertex
  void resetBottomMyConnectionOfActiveVertexPos();
  bool getBottomMyConnectionOfActiveVertexGoForward( TMyConnectionData * );
  //get right connection data of selected vertex
  void resetRightMyConnectionOfActiveVertexPos();
  bool getRightMyConnectionOfActiveVertexGoForward( TMyConnectionData * );

  //interface of different DR tools, such as greedy router and maze router
  bool addActiveVertexVirtualPin(string netName,  string layerName, long int x, long int y);
  bool clearActiveVertexVirtualPins(string layerName);
  void resetActiveVertexVirtualPinListPos();
  bool hasAnyVirtualPinInActiveVertexVirtualPinList();
  bool activeVertexVirtualPinListGoForward();
  string getActiveVertexVirtualPinNetName();
  string getActiveVertexVirtualPinLayerName();
  long int getActiveVertexVirtualPinX();
  long int getActiveVertexVirtualPinY();

  void assignLayoutToBinsToMesh();

  //select a layer of selected vertex
  void SelectLayerOfActiveVertex(string layerName);

  //get all of rects of a selected vertex (rects of layout)
  void resetListOfRectOfActiveVertex();
  bool getRectDataOfActiveVertexGoForward(TRectData *aRect);

  //get all of paths of a selected vertex (paths of layout)
  void resetListOfPathOfActiveVertex();
  bool resetListOfRectOfPathOfActiveVertex();
  bool getRectOfPathOfActiveVertexGoForward(TRectData *aRect);
  bool pathOfActiveVertexGoForward();
  long int getNumPointsOfPathOfAvtiveVertex();

  //get all of polygons of a selected vertex (polygons of layout)
  void resetListOfPolygonOfActiveVertex();
  bool resetListOfRectOfPolygonOfActiveVertex();
  bool getRectOfPolygonOfActiveVertexGoForward(TRectData *aRect);
  bool polygonOfActiveVertexGoForward();
  long int getNumPointsOfPolygonOfActiveVertex();

  //get instances of a selected vertex
  bool resetActiveVertexInstanceListPos();
  bool hasAnyInstanceInActiveVertexInstanceList();
  bool activeVertexInstanceListPosGoForward();
  string getActiveVertexInstanceName();

  //get nets of a selected vertex
  bool resetNetOfActiveVertex();
  bool getNetDataOfActiveVertex( TNetData *data );
  bool netOfActiveVertexGoForward();

  //get pin of previos net in selected vertex
  bool resetPinOfActiveNetOfActiveVertex();
  bool getPinDataOfActiveNetOfActiveVertex( TPinData *data );
  bool pinOfActiveNetOfActiveVertexGoForward();

  //get port of previos pin of previos net in selected vertex
  bool resetPortOfActivePinOfActiveVertex();
  bool getPortDataOfActivePinOfActiveVertex( TPortData *data );
  bool portOfActivePinOfActiveVertexGoForward();

  bool setLayerOfVertexNetPinPort(string layerName);
  bool hasAnyGeometryInVertexNetPinPort();

  //get rect data of previos port of previos pin of previos net in selected vertex
  bool resetRectOfActivePortOfActiveVertex();
  bool getRectDataOfActivePortOfActiveVertexGoForward( TRectData *data );

  //get path data of previos port of previos pin of previos net in selected vertex
  bool resetPathOfActivePortOfActiveVertex();
  bool getPathDataOfActivePortOfActiveVertex( TPathEndExt *type, long int *width );
  bool getRectDataOfPathOfActivePortOfActiveVertexGoForward( TRectData *data );
  bool pathOfActivePortOfActiveVertexGoForward();

  //get polygon data of previos port of previos pin of previos net in selected vertex
  bool resetPolygonOfActivePortOfActiveVertex();
  bool getPolygonDataOfActivePortOfActiveVertex();
  bool getRectDataOfPolygonOfActivePortOfActiveVertexGoForward( TRectData *data );
  bool polygonOfActivePortOfActiveVertexGoForward();

  //reading overlaps, top
  void resetActiveVertexOverlapTopNetListPos();
  bool hasAnyNetInActiveVertexOverlapTopNetList();
  bool activeVertexOverlapTopNetListPosGoForward();
  string getActiveVertexOverlapTopNetName();
  void resetActiveVertexOverlapTopNetPinListPos();
  bool hasAnyPinInActiveVertexOverlapTopNetPinList();
  bool activeVertexOverlapTopNetPinListPosGoForward();
  string getActiveVertexOverlapTopNetPinName();
  //reading overlaps, top left
  void resetActiveVertexOverlapTopLeftNetListPos();
  bool hasAnyNetInActiveVertexOverlapTopLeftNetList();
  bool activeVertexOverlapTopLeftNetListPosGoForward();
  string getActiveVertexOverlapTopLeftNetName();
  void resetActiveVertexOverlapTopLeftNetPinListPos();
  bool hasAnyPinInActiveVertexOverlapTopLeftNetPinList();
  bool activeVertexOverlapTopLeftNetPinListPosGoForward();
  string getActiveVertexOverlapTopLeftNetPinName();
  //reading overlaps, top right
  void resetActiveVertexOverlapTopRightNetListPos();
  bool hasAnyNetInActiveVertexOverlapTopRightNetList();
  bool activeVertexOverlapTopRightNetListPosGoForward();
  string getActiveVertexOverlapTopRightNetName();
  void resetActiveVertexOverlapTopRightNetPinListPos();
  bool hasAnyPinInActiveVertexOverlapTopRightNetPinList();
  bool activeVertexOverlapTopRightNetPinListPosGoForward();
  string getActiveVertexOverlapTopRightNetPinName();
  //reading overlaps, bottom
  void resetActiveVertexOverlapBottomNetListPos();
  bool hasAnyNetInActiveVertexOverlapBottomNetList();
  bool activeVertexOverlapBottomNetListPosGoForward();
  string getActiveVertexOverlapBottomNetName();
  void resetActiveVertexOverlapBottomNetPinListPos();
  bool hasAnyPinInActiveVertexOverlapBottomNetPinList();
  bool activeVertexOverlapBottomNetPinListPosGoForward();
  string getActiveVertexOverlapBottomNetPinName();
  //reading overlaps, bottom left
  void resetActiveVertexOverlapBottomLeftNetListPos();
  bool hasAnyNetInActiveVertexOverlapBottomLeftNetList();
  bool activeVertexOverlapBottomLeftNetListPosGoForward();
  string getActiveVertexOverlapBottomLeftNetName();
  void resetActiveVertexOverlapBottomLeftNetPinListPos();
  bool hasAnyPinInActiveVertexOverlapBottomLeftNetPinList();
  bool activeVertexOverlapBottomLeftNetPinListPosGoForward();
  string getActiveVertexOverlapBottomLeftNetPinName();
  //reading overlaps, bottom right
  void resetActiveVertexOverlapBottomRightNetListPos();
  bool hasAnyNetInActiveVertexOverlapBottomRightNetList();
  bool activeVertexOverlapBottomRightNetListPosGoForward();
  string getActiveVertexOverlapBottomRightNetName();
  void resetActiveVertexOverlapBottomRightNetPinListPos();
  bool hasAnyPinInActiveVertexOverlapBottomRightNetPinList();
  bool activeVertexOverlapBottomRightNetPinListPosGoForward();
  string getActiveVertexOverlapBottomRightNetPinName();
  //reading overlaps, left
  void resetActiveVertexOverlapRightNetListPos();
  bool hasAnyNetInActiveVertexOverlapRightNetList();
  bool activeVertexOverlapRightNetListPosGoForward();
  string getActiveVertexOverlapRightNetName();
  void resetActiveVertexOverlapRightNetPinListPos();
  bool hasAnyPinInActiveVertexOverlapRightNetPinList();
  bool activeVertexOverlapRightNetPinListPosGoForward();
  string getActiveVertexOverlapRightNetPinName();
  //reading overlaps, right
  void resetActiveVertexOverlapLeftNetListPos();
  bool hasAnyNetInActiveVertexOverlapLeftNetList();
  bool activeVertexOverlapLeftNetListPosGoForward();
  string getActiveVertexOverlapLeftNetName();
  void resetActiveVertexOverlapLeftNetPinListPos();
  bool hasAnyPinInActiveVertexOverlapLeftNetPinList();
  bool activeVertexOverlapLeftNetPinListPosGoForward();
  string getActiveVertexOverlapLeftNetPinName();

  /**
  ==============================================================================
      creating reports: tools
  ==============================================================================
  **/
  
  bool setReportTool(string toolName, TToolType type);
  bool setReportToolInfo(atlasString name, atlasString info);
  bool setReportToolExecutionStatus(atlasString name, TToolExecutionStatus es);
  
  /**
  ==============================================================================
      reading reports: tools
  ==============================================================================
  **/
  long int getReportToolNum();  
  bool resetReportToolListPos();
  bool hasAnyReportInReportToolList();
  bool reportToolListPosGoForward();
  string getReportToolName();
  TToolType getReportToolType(atlasString name);
  atlasString getReportToolInfo(atlasString name);
  TToolExecutionStatus getReportToolExecutionStatus(atlasString name);
  
  /**
  ==============================================================================
      File interface ( read/write, text/binary )
      to be used by : UI
  ==============================================================================
  **/
  // File dumping
  void WriteBinFile();
  void ReadBinFile();
  void setFilename( char * );
  const char * getFilename();

  // Print DB entries
  //void PrintDB(char *);
  //1long int hpWireLength();
  char *TEdgeStr(TEdge Edge);
  void PrintGR(char *fileName);
  //list < atlasLibPin *> *GetPinsIteratorOfMacro(atlasString macName);
  long int getViaToViaPitch();
  bool setWidthToHeightDieAspectRatio(float aspectRatio, float whiteSpace);//as is x/y
  long int getFloorPlanWidth();
  long int getFloorPlanHeight();
  int getVerticalPitch();
  int getHorizontalPitch();
  int view();

};


#endif //ATLAS_DB
