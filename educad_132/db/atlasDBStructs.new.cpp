#include "atlasDBStructs.h"


char *TPointData::ToString()
{
	char *array = new char[100];
	sprintf( "%d %d", array, top, left);
	return array;
}

char *TLayerData::ToString()
{
	char *array = new char[200];
	sprintf( "%s %d %d %d %d %d %d %f %f %f %f", array, name.c_str(), type, spacing, direction, pitch, width, offset, AcCurrentDensity, DcCurrentDensity, capacitance, resistance);
	sprintf("%s", array, name.c_str());
	return array;
}

char *TViaData::ToString()
{
	char *array = new char[200];
	sprintf( "%s %s %s %s %d %d %d %d %d %d %d %d %d %d %d %d", array, name.c_str(), layer1.c_str(), layer2.c_str(), cutLayer.c_str(), layer1rect.top, layer1rect.left, layer1rect.bottom, layer1rect.right, layer2rect.top, layer2rect.left, layer2rect.bottom, layer2rect.right, cutLayerRect.top, cutLayerRect.left, cutLayerRect.bottom, cutLayerRect.right);
	return array;
}

/*char *TInstData::ToString()
{
	char *array = new char[100];
	sprintf( "%s %d %d %s %d %d", array, instName.c_str(), top, left, macroName.c_str(), width, height );
	return array;
}*/

/*char *TNetData::ToString()
{
	char *array=new char[100];
	strcpy( array, name.c_str());
	
	return array;
}*/

/*char *TPinData::ToString()
{
	char *array = new char[100];
	sprintf( "%s %d", array, name.c_str(), dir );
	return array;
}*/

/*char *TPortData::ToString()
{
	char *array = new char[100];
	sprintf( "%d", array, type);
	return array;
}*/

char *TPlaceResults::ToString()
{
	char *array = new char[10];
	sprintf( "%d", array, placed );
	return array;
}
TRectData::TRectData()
{
	top = -1;
	left = -1;
	bottom = -1;
	right = -1;
}

char *TRectData::ToString()
{
	char *array = new char[100];
	sprintf( "%d %d %d %d", array, top, left, bottom, right );
	return array;
}

/*char *TNetlistData::ToString()
{
	char *array = new char[100];
	sprintf( "%d %d %d %d %d", array, Width, Height, RowNumber, RowHeight, Placed );
	return array;
}*/

char *TBinData::ToString()
{
	char *array = new char[100];
	sprintf( "%d %d %d %d", array, rowNum, colNum, binHeight, binWidth );
	return array;
}

TMacroParameters::TMacroParameters()
{
	 subType_valid = false;
	 foreign_valid = false;
	 foreignPnt_valid = false;
	 foreignOrient_valid = false;
	 siteName_valid = false;
	 size_valid = false; //width & height are valid
	 sEEQ_valid = false;
	 sLEQ_valid = false;
	 origin_valid = false;
	 type = macro_core_type;
	 subType = macro_bump_type;
	 source = macro_none_source;
	 foreignOrient = n_orientation;
	 width = -1;
	 height = -1;
	 symmetry = none_symmetry;
}

/*void TMacroParameters::setMacroType( TMacroType _type )
{
	macroType = _type;
}
void TMacroParameters::setMacroTType( TMacroTType _ttype )
{
	ttype = _ttype;
}
void TMacroParameters::setMacroSource( TMacroSource _source )
{
	macroSource = _source;
}
void TMacroParameters::setMacroForeign( string _macroForeign )
{
	macroForeign = _macroForeign;
}
void TMacroParameters::setMacroForeignPnt( TPointData _macroForeignPnt )
{
	macroForeignPnt = _macroForeignPnt;
}
void TMacroParameters::setMacroForeignOrient( string _macroOrient )
{
	macroForeignOrient = _macroOrient;
}
void TMacroParameters::setMacroSiteName( string _macroSiteName )
{
	macroSiteName = _macroSiteName;
}
void TMacroParameters::setMacroWidth( long int _width )
{
	width = _width;
}
void TMacroParameters::setMacroHeight( long int _height )
{
	height = _height;
}
void TMacroParameters::setMacroEEQ( string _eeq )
{
	sEEQ = _eeq;
}
void TMacroParameters::setMacroLEQ( string _leq )
{
	sLEQ = _leq;
}
void TMacroParameters::setMacroOrigin( TPointData _origin )
{
	origin = _origin;
}
void TMacroParameters::setMacroSymmetry( TMacroSymmetry _symmetry )
{
	symmetry = _symmetry;
}
void TMacroParameters::setMacroObstruction( list < TLayerGeometry > & _obstruction )
{
	obstructions = _obstruction;
}
void TMacroParameters::setMacroPins( list < TPinMacroParameters > & _pins )
{
	pins = _pins;
}
bool TMacroParameters::getMacroType( TMacroType & _type )
{
	if( type_valid )
	{
		_type = macroType;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroTType( TMacroTType & _ttype )
{
	if( ttype_valid )
	{
		_ttype = ttype;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroSource( TMacroSource & _source )
{
	if( source_valid )
	{
		_source = macroSource;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroForeign( string & _macroForeign )
{
	if( foreign_valid )
	{
		_macroForeign = macroForeign;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroForeignPnt( TPointData & _macroForeignPnt )
{
	if( foreignPnt_valid )
	{
		_macroForeignPnt = macroForeignPnt;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroForeignOrient( string & _macroOrient )
{
	if( foreignOrient_valid )
	{
		_macroOrient  = macroForeignOrient;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroSiteName( string & _macroSiteName )
{
	if( siteName_valid )
	{
		_macroSiteName = macroSiteName;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroWidth( long int & _width )
{
	if( size_valid )
	{
		_width = width;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroHeight( long int & _height )
{
	if( size_valid )
	{
		_height = height;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroEEQ( string & _eeq )
{
	if( eeq_valid )
	{
		_eeq = sEEQ;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroLEQ( string & _leq )
{
	if( leq_valid )
	{
		_leq = sLEQ;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroOrigin( TPointData & _origin )
{
	if( origin_valid )
	{
		_origin = origin;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroSymmetry( TMacroSymmetry & _symmetry )
{
	if( symmetry_valid )
	{
		_symmetry = symmetry;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroObstruction( list < TLayerGeometry > & _obstruction )
{
	if( obstructions.size() )
	{
		_obstruction = obstructions;
		return true;
	}
	return false;
}
bool TMacroParameters::getMacroPins( list < TPinMacroParameters > & _pins )
{
	if( pins.size() )
	{
		_pins = pins;
		return true;
	}
	return false;
}
*/
TRLminCut::TRLminCut()
{
	length_valid = false;
	distance_valid = false;
}

TRLminEncArea::TRLminEncArea()
{
	width_valid = false;
}

TRLAcCurrentDensity::TRLAcCurrentDensity()
{
	value_valid = false;
	table_valid = false;
}

TRLDcCurrentDensity::TRLDcCurrentDensity()
{
	value_valid = false;
	table_valid = false;
}

TRLminSpacing::TRLminSpacing()
{
	spacing_Range_valid = false;
	spacing_Range_UseLengthThreshold_valid = false;
	spacing_Range_Influence_valid = false;
	spacing_Range_Influence_Range_valid = false;
	spacing_Range_Range_valid = false;
	spacing_LengthThreshold_valid = false;
	spacing_LengthThreshold_Range_valid = false;
}

TRLspacingTable::TRLspacingTable()
{
	influence_table_valid = false;
}

TRLspacing::TRLspacing()
{
	spacing_valid = false;
	spacingTable_valid = false;
}

TRLparameters::TRLparameters()
{
	 offset_valid = false;
	 minArea_valid = false;
	 spacing_valid = false;
	 wireExtension_valid = false;
	 max_width_valid = false;
	 min_width_valid = false;
	 protrusion_valid = false;
	 resistance_valid = false;
	 capacitance_valid = false;
	 height_valid = false;
	 thickness_valid = false;
	 shrinkage_valid = false;
	 cap_multiplier_valid = false;
	 edge_capacitance_valid = false;
	 slot_min_wire_width_valid = false;
	 slot_min_wire_length_valid = false;
	 slot_min_width_valid = false;
	 slot_min_length_valid = false;
	 max_adj_slot_spacing_valid = false;
	 max_coaxial_slot_spacing_valid = false;
	 max_edge_slot_spacing_valid = false;
	 split_wire_width_valid = false;
	 min_dencity_valid = false;
	 max_dencity_valid = false;
	 density_check_window_valid = false;
	 density_check_step_valid = false;
	 min_step_valid = false;
	 fill_active_spacing_valid = false;
}

TCLspacing::TCLspacing()
{
	layer2name_valid = false;
	adjacent_valid = false;
}

TCLacCurrentDensity::TCLacCurrentDensity()
{
	value_valid = false;
	table_valid = false;
}

TCLdcCurrentDensity::TCLdcCurrentDensity()
{
	value_valid = false;
	table_valid = false;
}

TILspacing::TILspacing()
{
	layer2name_valid = false;
}

TILparameters::TILparameters()
{
	width_valid = false;
}

TLayerGeometry::TLayerGeometry()
{
	spacing_valid = false;
	designRuleWidth_valid = false;
	spacing = -1;
	designRuleWidth = -1;
}

TPortParameters::TPortParameters()
{
	pClass_valid = false;
	pClass = port_none_class;
}

TPinMacroParameters::TPinMacroParameters()
{
	taperRule_valid = false;
	foreignPinName_valid = false;
	foreignPnt_valid = false;
	foreignOrient_valid = false;
	direction_valid = false;
	use_valid = false;
	shape_valid = false;
	mustJoin_valid = false;
	outNoiseMargin_valid = false;
	outResistance_valid = false;
	inNoiseMargin_valid = false;
	power_valid = false;
	leakage_valid = false;
	maxLoad_valid = false;
	capacitance_valid = false;
	resistance_valid = false;
	pullDownRes_valid = false;
	tieoffr_valid = false;
	VHI_valid = false;
	VLO_valid = false;
	riseVoltage_valid = false;
	fallVoltage_valid = false;
	riseThresh_valid = false;
	fallThresh_valid = false;
	riseSatcur_valid = false;
	fallSatcur_valid = false;
	riseSlewLimit_valid = false;
	fallSlewLimit_valid = false;
	currentSource_valid = false;
	tableName_valid = false;
	foreignOrient = n_orientation;
	direction = pin_input_dir;
	use = pin_none_use;
	shape = pin_none_shape;
	outNoiseMarginLow = -1.0;
	outNoiseMarginHigh = -1.0;
	outResistanceLow = -1.0;
	outResistanceHigh = -1.0;
	inNoiseMarginLow = -1.0;
	inNoiseMarginHigh = -1.0;
	power = -1.0;
	leakage = -1.0;
	maxLoad = -1.0;
	capacitance = -1.0;
	resistance = -1.0;
	pullDownRes = -1.0;
	tieoffr = -1.0;
	VHI = -1.0;
	VLO = -1.0;
	riseVoltage = -1.0;
	fallVoltage = -1.0;
	riseThresh = -1.0;
	fallThresh = -1.0;
	riseSatcur = -1.0;
	fallSatcur = -1.0;
	riseSlewLimit = -1.0;
	fallSlewLimit = -1.0;
}

TMaxViaStack::TMaxViaStack()
{
	range_valid = false;
	maxViaStack = -1;
}

TSiteParameters::TSiteParameters()
{
	symmetry_valid = false;
	sClass = site_none_class;
	symmetry = none_symmetry;
	width = -1;
	height = -1;
}

TSameNetSpacing::TSameNetSpacing()
{
	stackAllowd_valid = false;
	minSpacing = -1;
	stackAllowd = false;
}

TUnitParameters::TUnitParameters()
{
	timeUnit_valid = false;
	capacitanceUnit_valid = false;
	resistanceUnit_valid = false;
	powerUnit_valid = false;
	currentUnit_valid = false;
	voltageUnit_valid = false;
	databaseUnit_valid = false;
	frequencyUnit_valid = false;
	timeUnit = none_unit;
	capacitanceUnit = none_unit;
	resistanceUnit = none_unit;
	powerUnit = none_unit;
	currentUnit = none_unit;
	voltageUnit = none_unit;
	databaseUnit = none_unit;
	frequencyUnit = none_unit;
	
	timeConvertFactor = -1;
	capacitanceConvertFactor = -1;
	resistanceConvertFactor = -1;
	powerConvertFactor = -1;
	currentConvertFactor = -1;
	voltageConvertFactor = -1;
	databaseConvertFactor = -1;
	frequencyConvertFactor = -1;
}

TViaParameters::TViaParameters()
{
	 defaultVia = false;
	 topOfStackOnly = false;
	 foreignCellName_valid = false;
	 foreignPnt_valid = false;
	 foreignOrient_valid = false;
	 resistance_valid = false;
	 foreignOrient = n_orientation;
}

TViaRuleLayer::TViaRuleLayer()
{
	width_valid = false;
	overhang_valid = false;
	metalOverhang_valid = false;
	enclosure_valid = false;
	layerDirection = layer_horizontal_dir;
	minWidth = -1;
	maxWidth = -1;
	overhang = -1;
	metalOverhang = -1;
	enclosure_overhang1 = -1;
	enclosure_overhang2 = -1;
}

TViaRule::TViaRule()
{
	generate = false;
	resistance_valid = false;
	xSpacing = -1;
	ySpacing = -1;
	resistance = -1.0;
}

TPointData::TPointData()
{
	top = -1;
	left = -1;
}

TPathData::TPathData()
{
	extType = extend;
	width = -1;
}

