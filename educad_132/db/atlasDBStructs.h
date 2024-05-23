/**
        Atlas DB basic structures

        This structures are required by interface methods

        Updating History: 83/11/11

        Non-Implemented Items:
**/

#ifndef ATLAS_DB_STRUCTS_H
#define ATLAS_DB_STRUCTS_H

#include "atlasString.h"
#include <string>
#include <map>
#include <list>
using namespace std;

/*#include "atlasLibTypes.h"
//#include "atlasDsnTypes.h"
#include "atlasTchTypes.h"
#include "atlasGeomTypes.h"*/

enum TEdge { edTop,edBottom, edLeft, edRight  };
enum TInstPlaceType {unplaced_inst, placed_inst, fixed_inst, cover_inst};
enum TPathEndExt { extend, not_extend };
enum TUnitType {none_unit, nanoseconds, picofarads, ohms, milliwatts, milliamps, volts, microns, megahertz};
enum TMacroType { macro_cover_type, macro_cover_bump_type, macro_ring_type, macro_block_type, macro_block_blackBox_type, macro_pad_type, macro_pad_input_type, macro_pad_output_type, macro_pad_inout_type, macro_pad_power_type, macro_pad_spacer_type, macro_pad_areaIO_type, macro_core_type, macro_core_feedThru_type, macro_core_tieHigh_type, macro_core_tieLow_type, macro_core_spacer_type, macro_core_antennaCell_type, macro_endcap_type, macro_endcap_pre_type, macro_endcap_post_type, macro_endcap_topLeft_type, macro_endcap_topRight_type, macro_endcap_bottomLeft_type, macro_endcap_bottomRight_type };
enum TMacroSubType {macro_bump_type, macro_blackBox_type, macro_input_type, macro_output_type, macro_inout_type, macro_power_type, macro_spacer_type, macro_areaIO_type, macro_feedThru_type, macro_tieHigh_type, macro_tieLow_type, macro_antennaCell_type, macro_pre_type, macro_post_type, macro_topLeft_type, macro_topRight_type, macro_bottomLeft_type, macro_bottomRight_type};
enum TMacroSymmetry { none_symmetry, x_symmetry, y_symmetry, r90_symmetry, xy_symmetry, xr90_symmetry, yr90_symmetry, xyr90_symmetry};
enum TMacroSource { macro_none_source, macro_user_source, macro_block_source};
enum TPinDirection { pin_input_dir, pin_output_dir, pin_tristate_dir, pin_inout_dir, pin_feedthru_dir };
enum TPinUse { pin_none_use, pin_signal_use, pin_analog_use, pin_clock_use, pin_ground_use, pin_power_use, pin_tieoff_use, pin_scan_use, pin_reset_use};
enum TPinShape { pin_none_shape, pin_abutment_shape, pin_ring_shape, pin_feedthru_shape};
//enum TPinShape { pin_none_shape, abutment_shape, ring_shape, core_shape};
enum TPortClass { port_none_class, port_core_class};
enum TSiteClass { site_none_class, site_pad_class, site_core_class};
enum TLayerType { masterSlice, cut, routing, implant, overlap};
enum TLayerMastersliceType {mastersliceType, overlapType};
enum TNetSource { net_none_source, net_dist_source, net_netlist_source, net_test_source, net_timing_source, net_user_source};
enum TNetUse { net_none_use, net_signal_use, net_analog_use, net_clock_use, net_ground_use, net_power_use, net_reset_use, net_scan_use, net_tieoff_use };
enum TNetPattern { net_none_pattern, net_balanced_pattern, net_steiner_pattern, net_trunk_pattern, net_wiredlogic_pattern};
enum TGlobalPinPlaceType{gpin_unplaced_placeType, gin_placed_placeType, gpin_fixed_placeType, gpin_cover_placeType};
//----------------------------------  tech routing layer 
enum TRLayerDirection { layer_horizontal_dir, layer_vertical_dir };
enum TFromSide {both_side, FromAbove, FromBelow};
enum TOrientation {n_orientation, s_orientation, w_orientation, e_orientation, fn_orientation, fs_orientation, fw_orientation, fe_orientation};
enum TRegionType {region_core_type, region_pad_type, region_corner_type};
enum TToolType {tool_none_type, tool_floorplaner_type, tool_placer_type, tool_router_type, tool_analysis_type};
enum TToolExecutionStatus {tool_none_es, tool_notRunning_es, tool_run_failed_es, tool_run_warning_es, tool_run_ok_es };

class TRLminCut{
public:
	TRLminCut();
	long int minCut;
	long int width;
	TFromSide fromSide;
	bool length_valid;
	long int length;
	bool distance_valid;
	long int distance;
};

class TRLminEncArea{
public:
	TRLminEncArea();
	long int area;
	bool width_valid;
	long int width;
};

class TRLprotrusion{
public:
	long int width1;
	long int length;
	long int width2;
};

class TRLdensityCheckWindow{
public:
	long int window_length;
	long int window_width;
};

enum TcurrentDensityType {density_peak, density_average, density_rms};
/*Class TRLacTableEntries{
public:
	float value;
	float frequency;
	long int width;
};*/
class TRLAcCurrentDensity{
public:
	TRLAcCurrentDensity();
	TcurrentDensityType type;
	bool value_valid;
	float value;
	bool table_valid;
	list <float> frequency;
	list <long int> width;
	list <float> tableEntries;
};

class TRLDcCurrentDensity{
public:
	TRLDcCurrentDensity();
	TcurrentDensityType type;
	bool value_valid;
	float value;
	bool table_valid;
	list <long int> width;
	list <float> tableEntries;
};

/*Class TRLspacingOpt2{
public:
	bool use_length_thre_valid;
	
	bool influence_valid;
	long int influence;
	bool influence_range_valid;
	long int stub_min_width;
	long int stub_max_width;
	
	bool range_valid;
	long int min_width;
	long int max_width;
};
Class TRLspacingOpt1{
public:
	bool range_valid;
	long int min_width;
	long int max_width;
	
	bool opt2_valid;
	TRLspacingOpt2 opt2;
	
	bool length_threshold_valid;
	long int length_threshold;
	bool len_thre_range_valid;
	long int len_thre_min_width;
	long int len_thre_max_width;
};*/
class TRLminSpacing{
public:
	TRLminSpacing();
	long int minSpacing;
	
	bool spacing_Range_valid;
	long int spacing_Range_min;
	long int spacing_Range_max;
	
	bool spacing_Range_UseLengthThreshold_valid;
	
	bool spacing_Range_Influence_valid;
	long int spacing_Range_Influence;
	
	bool spacing_Range_Influence_Range_valid;
	long int spacing_Range_Influence_Range_min;
	long int spacing_Range_Influence_Range_max;
	
	bool spacing_Range_Range_valid;
	long int spacing_Range_Range_min;
	long int spacing_Range_Range_max;
	
	bool spacing_LengthThreshold_valid;
	long int spacing_LengthThreshold;
	
	bool spacing_LengthThreshold_Range_valid;
	long int spacing_LengthThreshold_Range_min;
	long int spacing_LengthThreshold_Range_max;
};
class TRLpara_run_len_entry{
public:
	long int length;
	long int width;
	long int spacing;
};
class TRLinfluence_entry{
public:
	long int width;
	long int distance;
	long int spacing;
};
class TRLspacingTable{
public:
	TRLspacingTable();
	list <TRLpara_run_len_entry *> para_run_len_table;
	bool influence_table_valid;
	list <TRLinfluence_entry *> influence_table;
};
class TRLspacing{
public:
	TRLspacing();
	bool spacing_valid;
	list <TRLminSpacing *> spacing;
	bool spacingTable_valid;
	TRLspacingTable spacingTable;
};

class TRLparameters{
public:
	TRLparameters();
	TRLayerDirection direction;
	long int pitch;
	long int width;
	
	long int offset;
	long int minArea;
	TRLspacing spacing;
	long int wireExtension;
	list <TRLminCut *> minCut;
	long int max_width;
	long int min_width;
	list <TRLminEncArea *> min_enclosed_area;
	TRLprotrusion protrusion;
	float resistance;
	float capacitance;
	long int height;
	long int thickness;
	long int shrinkage;
	long int cap_multiplier;
	float edge_capacitance;
	long int slot_min_wire_width;
	long int slot_min_wire_length;
	long int slot_min_width;
	long int slot_min_length;
	long int max_adj_slot_spacing;
	long int max_coaxial_slot_spacing;
	long int max_edge_slot_spacing;
	long int split_wire_width;
	long int min_dencity;
	long int max_dencity;
	TRLdensityCheckWindow density_check_window;
	long int density_check_step;
	long int min_step;
	long int fill_active_spacing;
	list <TRLAcCurrentDensity *> ac_current_density;
	list <TRLDcCurrentDensity *> dc_current_density;
	
	bool offset_valid;
	bool minArea_valid;
	bool spacing_valid;
	bool wireExtension_valid;
	bool max_width_valid;
	bool min_width_valid;
	bool protrusion_valid;
	bool resistance_valid;
	bool capacitance_valid;
	bool height_valid;
	bool thickness_valid;
	bool shrinkage_valid;
	bool cap_multiplier_valid;
	bool edge_capacitance_valid;
	bool slot_min_wire_width_valid;
	bool slot_min_wire_length_valid;
	bool slot_min_width_valid;
	bool slot_min_length_valid;
	bool max_adj_slot_spacing_valid;
	bool max_coaxial_slot_spacing_valid;
	bool max_edge_slot_spacing_valid;
	bool split_wire_width_valid;
	bool min_dencity_valid;
	bool max_dencity_valid;
	bool density_check_window_valid;
	bool density_check_step_valid;
	bool min_step_valid;
	bool fill_active_spacing_valid;
};
//----------------------------------  tech cut layer 
class TCLspacing{
public:
	TCLspacing();
	long int spacing;
	
	bool layer2name_valid;
	string layer2name;
	bool adjacent_valid;
	long int adjacent;
	long int distance;
};

class TCLacCurrentDensity{
public:
	TCLacCurrentDensity();
	TcurrentDensityType type;
	bool value_valid;
	float value;
	bool table_valid;
	list <float> frequency;
	list <long int> cutArea;
	list <float> tableEntries;
};

class TCLdcCurrentDensity{
public:
	TCLdcCurrentDensity();
	TcurrentDensityType type;
	bool value_valid;
	float value;
	bool table_valid;
	list <long int> cutArea;
	list <float> tableEntries;
};

class TCLparameters{
public:
	list <TCLspacing*> spacing;
	list <TCLacCurrentDensity *> acCurrentDensity;
	list <TCLdcCurrentDensity *> dcCurrentDensity; //in complete lef more than one dc current density present;
};
//---------------------------------- tech implant layer
class TILspacing{
public:
	TILspacing();
	long int minSpacing;
	
	bool layer2name_valid;
	string layer2name;
};

class TILparameters{
public:
	TILparameters();
	long int width;
	list <TILspacing *> spacing;
	
	bool width_valid;
};
//--------------------------------------------------------------
enum TPosition { BEGIN, END };
enum TMovement { NONE, FORWARD };

class TPointData{
public:
	TPointData();
	long int top;
	long int left;
	char *ToString();
};


struct TTerminalData{
	string pinName;
	TPinDirection pinDir;
	TPortClass type;
	long int x;
	long int y;
};

class TLayerData {
public:
	string name;
	TLayerType type;
	long int spacing;
	TRLayerDirection direction;
	long int pitch;
	long int width;
	long int offset;
	float AcCurrentDensity;
	float DcCurrentDensity;
	float capacitance;
	float resistance;
	char *ToString();
};

class TRectData {
public:
	TRectData();
	long int top;
	long int left;
	long int bottom;
	long int right;
	char *ToString();
};

class TViaData {
public:
	string name;
	string layer1;
	string layer2;
	string cutLayer;
	TRectData layer1rect;
	TRectData layer2rect;
	TRectData cutLayerRect;
	char *ToString();
};

/*class TMacroData {
public:
	string name;
	TMacroType macroType;
	TMacroSubType subType;
	long int width;
	long int height;
};*/

/*class TInstData {
public:
	string instName;
	long int top;
	long int left;
	string macroName;
	long int width;
	long int height;
	TMacroSubType type; // ???????
	char *ToString();
};*/

class TNetData{
public:
	string name;
	TNetUse use;
	char *ToString();
};

class TPinData {
public:
	string name;
	TPinDirection dir;
	TPinUse use;
	string parentName;
	long int parentTop;////////////////////////////////
	long int parentLeft;//////////////////////////
	char *ToString();
};

class TPortData {
public:
	TPortClass type;
	char *ToString();
};

class TPlaceResults {
public:
  bool placed;
  char *ToString();
};



/*class TNetlistData {
public:
	long int Width;
	long int Height;
	long int RowNumber;
	long int RowHeight;
	bool Placed;
	char *ToString();
};*/

class TBinData {
public:
	long int rowNum;                 // number of bin rows in plane
	long int colNum;                 // number of bin columns in plane
	long int binHeight;              // height of each bin in design unit
	long int binWidth;               // width of each bin in design unit
	char *ToString();
};

struct TConnectionData {
	string netName;
	TEdge firstEdge;
	long int firstEdgeCP;
	string firstLayer;
	TEdge secondEdge;
	long int secondEdgeCP;
	string secondLayer;
};

struct TMyConnectionData {
	string netName;
	string layerName;
	long int netAssignedTrack;
    
	TMyConnectionData()
	{
        netAssignedTrack = -1; 
    }
};

//---------------------------------- macro
//---------------------------------- macro pin
class TPathData{
public:
	TPathData();
	TPathEndExt extType;
	long int width;
	list < TPointData *> points;
};
class TPolygonData{
public:
	list < TPointData *> points;
};
class TViaUsed{
public:
	string name;
	TPointData point;
};
class TLayerGeometry{
public:
	TLayerGeometry();
	string layerName;
	bool spacing_valid;
	long int spacing;
	bool designRuleWidth_valid;
	long int designRuleWidth;
	list < TRectData *> rects;
	list < TPathData *> paths;
	list < TPolygonData *> polygons;
	list < TViaUsed *> via;
};

class TPortParameters{
public:
	TPortParameters();
	TPortClass pClass;
	list < TLayerGeometry *> geometry;
	
	bool pClass_valid;
};
class TPinMacroParameters{
public:
	TPinMacroParameters();
	string pinName;
	string taperRule;
	string foreignPinName;
	TPointData foreignPnt;
	TOrientation foreignOrient;
	list < string > LEQs;
	TPinDirection direction;
	TPinUse use;
	TPinShape shape;
	string mustJoin;
	list < TPortParameters *> ports;
	float outNoiseMarginLow;
	float outNoiseMarginHigh;
	float outResistanceLow;
	float outResistanceHigh;
	float inNoiseMarginLow;
	float inNoiseMarginHigh;
	float power;
	float leakage;
	float maxLoad;
	float capacitance;
	float resistance;
	float pullDownRes;
	float tieoffr;
	float VHI;
	float VLO;
	float riseVoltage;
	float fallVoltage;
	float riseThresh;
	float fallThresh;
	float riseSatcur;
	float fallSatcur;
	float riseSlewLimit;
	float fallSlewLimit;
	string currentSource;
	string tableHighName;
	string tableLowName;
	
	bool taperRule_valid;
	bool foreignPinName_valid;
	bool foreignPnt_valid;
	bool foreignOrient_valid;
	bool direction_valid;
	bool use_valid;
	bool shape_valid;
	bool mustJoin_valid;
	bool outNoiseMargin_valid;
	bool outResistance_valid;
	bool inNoiseMargin_valid;
	bool power_valid;
	bool leakage_valid;
	bool maxLoad_valid;
	bool capacitance_valid;
	bool resistance_valid;
	bool pullDownRes_valid;
	bool tieoffr_valid;
	bool VHI_valid;
	bool VLO_valid;
	bool riseVoltage_valid;
	bool fallVoltage_valid;
	bool riseThresh_valid;
	bool fallThresh_valid;
	bool riseSatcur_valid;
	bool fallSatcur_valid;
	bool riseSlewLimit_valid;
	bool fallSlewLimit_valid;
	bool currentSource_valid;
	bool tableName_valid;
};


class TMacroParameters{
public:
	TMacroParameters();
public:
	TMacroType type;
	TMacroSubType subType;
	TMacroSource source;
	string foreign;
	TPointData foreignPnt;
	TOrientation foreignOrient;
	string siteName;
	long int width;
	long int height;
	string	sEEQ;
	string	sLEQ;
	TPointData origin;
	TMacroSymmetry symmetry;
	list < TLayerGeometry *> obstructions;
	list < TPinMacroParameters *> pins;
	
	bool subType_valid;
	bool foreign_valid;
	bool foreignPnt_valid;
	bool foreignOrient_valid;
	bool siteName_valid;
	bool size_valid; //width & height are valid
	bool sEEQ_valid;
	bool sLEQ_valid;
	bool origin_valid;
};
//----------------------- max via stack 
class TMaxViaStack{
public:
	TMaxViaStack();
	long int maxViaStack;
	bool range_valid;
	string firstLayer;
	string secondLayer;
};
//----------------------- site
class TSiteParameters{
public:
	TSiteParameters();
	TSiteClass sClass;
	TMacroSymmetry symmetry;
	long int width;
	long int height;
	bool symmetry_valid;
};
//---------------------- same net spacing
class TSameNetSpacing{
public:
	TSameNetSpacing();
	string firstLayerName;
	string secondLayerName;
	long int minSpacing;
	bool stackAllowd;
	bool stackAllowd_valid;
};
class TSameNetSpacingParameters{
public:
	list < TSameNetSpacing *> sameNetSpacing;
};
//---------------------- units
class TUnitParameters{
public:
	TUnitParameters();
	TUnitType timeUnit;
	TUnitType capacitanceUnit;
	TUnitType resistanceUnit;
	TUnitType powerUnit;
	TUnitType currentUnit;
	TUnitType voltageUnit;
	TUnitType databaseUnit;
	TUnitType frequencyUnit;
	
	long int timeConvertFactor;
	long int capacitanceConvertFactor;
	long int resistanceConvertFactor;
	long int powerConvertFactor;
	long int currentConvertFactor;
	long int voltageConvertFactor;
	long int databaseConvertFactor;
	long int frequencyConvertFactor;
	
	bool timeUnit_valid;
	bool capacitanceUnit_valid;
	bool resistanceUnit_valid;
	bool powerUnit_valid;
	bool currentUnit_valid;
	bool voltageUnit_valid;
	bool databaseUnit_valid;
	bool frequencyUnit_valid;
};
//----------------------- via
class TViaLayerRects{
public:
	string layerName;
	list <TRectData *> rects;
};
class TViaParameters{
public:
	TViaParameters();
	bool defaultVia;
	bool topOfStackOnly;
	string foreignCellName;
	TPointData foreignPnt;
	TOrientation foreignOrient;
	float resistance;
	list <TViaLayerRects *> layers;
	
	bool foreignCellName_valid;
	bool foreignPnt_valid;
	bool foreignOrient_valid;
	bool resistance_valid;
};
//---------------------- via rule
class TViaRuleLayer{
public:
	TViaRuleLayer();
	string layerName;
	TRLayerDirection layerDirection;
	long int minWidth;
	long int maxWidth;
	bool width_valid;
	
	long int overhang;
	bool overhang_valid;
	long int metalOverhang;
	bool metalOverhang_valid;
	long int enclosure_overhang1;
	long int enclosure_overhang2;
	bool enclosure_valid;
};
class TViaRule{
public:
	TViaRule();
	//via rule
	TViaRuleLayer layer1;
	TViaRuleLayer layer2;
	list <string> viaNames;
	
	//via rule generate
	bool generate;
	string cutLayerName;
	TRectData rect;
	long int xSpacing;
	long int ySpacing;
	float resistance;
	bool resistance_valid;
};

class TGlobalPinParameters{
public:
	TPinDirection direction;
	TPinUse use;
	bool special;
	bool layerName_valid;
	string pinGeomLayerName;
	TRectData pinGeomRect;
	TGlobalPinPlaceType placeType;
	TPointData placePoint;
	TOrientation placeOrientation;
};

#endif //ATLAS_DB_STRUCTS_H
