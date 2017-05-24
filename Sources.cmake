set(MSDQUALIF_Src_Files ${MSDQUALIF_DIR}/MSD_Qualif.cpp
)

SOURCE_GROUP(MSDQUALIF FILES ${MSDQUALIF_Src_Files})

set(Elise_Src_Files
	${Elise_Src_Files}
	${MSDQUALIF_Src_Files}
)
