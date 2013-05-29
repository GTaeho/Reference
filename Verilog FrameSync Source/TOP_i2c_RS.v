module TOP_i2c_RS(iCLK, iRST,
									oSCL_1, oSDA_1,
							 		oSCL_2, oSDA_2,
									oSCL_3, oSDA_3,
									oSCL_4, oSDA_4,
									oSCL_5, oSDA_5,
									oSCL_6, oSDA_6,
									oSCL_7, oSDA_7,
									oSCL_8, oSDA_8,
									O_txd, off_start, LED1, LED2,  
									oSCLK, oFSYNC, oXOUT, oYOUT, oZOUT, oARM_CLK,
									LCD_DATA, LCD_RW, LCD_EN, LCD_RS, beep_out);



	input 	iCLK, iRST, off_start;
	
	inout 	oSDA_1;
 	inout 	oSDA_2;
	inout 	oSDA_3;
	inout 	oSDA_4;
	inout 	oSDA_5;
	inout 	oSDA_6;
	inout 	oSDA_7;
	inout 	oSDA_8;
	
	output	beep_out;
	output 	oSCL_1; 
	output	oSCL_2;
	output	oSCL_3;
	output	oSCL_4;
	output	oSCL_5;
	output	oSCL_6;
	output	oSCL_7;
	output	oSCL_8;
	output  O_txd, LED1, LED2;
	/* FRAME_SYCN�� ���� 100Hz���� ���� ��� �����͸� ARM�� ������. */
	output	oSCLK, oFSYNC, oXOUT, oYOUT, oZOUT;
	output	oARM_CLK;
	//	LCD Module
	output	[7:0]	LCD_DATA;
	output	LCD_RW;
	output	LCD_EN;
	output	LCD_RS;
	
	

	/* BCD_data_x,y,z�� wire�� i2c ��⿡ ���� �� */
	wire 		[13:0]	BCD_data_x;
	wire		[13:0]	BCD_data_y;
	wire		[13:0]	BCD_data_z;
	/* i2c_flag�� i2c�� ���ͼ� OFF_ELI ���� ����. (OFF_ELI_Enable ����) */
	wire						i2c_flag;
	/* sum_flag�� OFF_ELI���� ���ͼ� SUM���� ����. (SUM_Enable ����)  */
	wire						sum_flag;
	/* bcd_flag�� SUM�� ���ͼ� BCD ���� ����. (BCD_Enable ����) */
	wire						bcd_flag;
	/* i2c ��� ������ 4EA ,(= OFF_ELI�� �Է� ������) */
	wire 		[15:0]	off_data_x_1;
	wire 		[15:0]	off_data_y_1;
	wire 		[15:0]	off_data_z_1;
	
	wire 		[15:0]	off_data_x_2;
	wire 		[15:0]	off_data_y_2;
	wire 		[15:0]	off_data_z_2;
	
	wire 		[15:0]	off_data_x_3;
	wire 		[15:0]	off_data_y_3;
	wire 		[15:0]	off_data_z_3;
	
	wire 		[15:0]	off_data_x_4;
	wire 		[15:0]	off_data_y_4;
	wire 		[15:0]	off_data_z_4;
	
	wire 		[15:0]	off_data_x_5;
	wire 		[15:0]	off_data_y_5;
	wire 		[15:0]	off_data_z_5;
	
	wire 		[15:0]	off_data_x_6;
	wire 		[15:0]	off_data_y_6;
	wire 		[15:0]	off_data_z_6;
	
	wire 		[15:0]	off_data_x_7;
	wire 		[15:0]	off_data_y_7;
	wire 		[15:0]	off_data_z_7;
	
	wire 		[15:0]	off_data_x_8;
	wire 		[15:0]	off_data_y_8;
	wire 		[15:0]	off_data_z_8;
	/* OFF_ELI ��� ������,(= SUM�� �Է� ������) */
	wire 		[13:0]	SUM_data_x_1;
	wire		[13:0]	SUM_data_y_1;
	wire		[13:0]	SUM_data_z_1;
	
	wire 		[13:0]	SUM_data_x_2;
	wire		[13:0]	SUM_data_y_2;
	wire		[13:0]	SUM_data_z_2;
	
	wire 		[13:0]	SUM_data_x_3;
	wire		[13:0]	SUM_data_y_3;
	wire		[13:0]	SUM_data_z_3;
	
	wire 		[13:0]	SUM_data_x_4;
	wire		[13:0]	SUM_data_y_4;
	wire		[13:0]	SUM_data_z_4;
	
	wire 		[13:0]	SUM_data_x_5;
	wire		[13:0]	SUM_data_y_5;
	wire		[13:0]	SUM_data_z_5;
	
	wire 		[13:0]	SUM_data_x_6;
	wire		[13:0]	SUM_data_y_6;
	wire		[13:0]	SUM_data_z_6;
	
	wire 		[13:0]	SUM_data_x_7;
	wire		[13:0]	SUM_data_y_7;
	wire		[13:0]	SUM_data_z_7;
	
	wire 		[13:0]	SUM_data_x_8;
	wire		[13:0]	SUM_data_y_8;
	wire		[13:0]	SUM_data_z_8;
	/////////////////////////////////////////////
	/* fsync��  FRAME_SYNC ��⿡�� 100Hz��ȣ���� i2c���� ��ȣ ���. */
	wire 						fsync;	
	/* RS232 Tx_enable */
	wire						Tx_en;
	/* o_txd�� �� ��Ʈ�� ��� �� �����͸� ����. */
	wire            o_txd;
	//////////////////////////////////////////////////////

	/* BCD_26bit�� ���� ���� ���̾�	*/
	wire		[25:0]	w_BCD_in_x;
	wire		[25:0]	w_BCD_in_y;
	wire		[25:0]	w_BCD_in_z;
	//////////////////////////////////////////////////////
		/* w_DATA_X,,Y,Z�� BCD���� ���ͼ� ASCII�� ��.. */
	wire 		[32:0]	w_BCD_DATA_X;
	wire 		[32:0]	w_BCD_DATA_Y;	
	wire 		[32:0]	w_BCD_DATA_Z;	
////////////////////////////////////////////////////////
/* �ƽ�Ű�ڵ�� 17��Ʈ�� ���� ���� ���̾�ó��*/
	wire		[16:0]	w_ASC_in_X;
	wire		[16:0]	w_ASC_in_Y;
	wire		[16:0]	w_ASC_in_Z;
	//////////////////////////////////////////////////////
		/* O_ASC_X,,Y,Z�� ASCII���� ���ͼ� RS_TX�� ��.. */
	wire 		[47:0]	O_ASC_X;
	wire 		[47:0]	O_ASC_Y;	
	wire 		[47:0]	O_ASC_Z;	
	//////////////////////////////////////////////////////
	/* 26bit BCD ���� ���� ���ؼ� ��Ʈ Ȯ���� ���� ����. [15]�� MSB��� ��ȣ Ȯ���̴� */
	assign	w_BCD_in_x = {BCD_data_x[13], BCD_data_x[13], BCD_data_x[13], BCD_data_x[13], BCD_data_x[13], BCD_data_x[13], BCD_data_x[13], 
											 BCD_data_x[13], BCD_data_x[13], BCD_data_x[13], BCD_data_x[13], BCD_data_x[13], BCD_data_x[13:0]};
	assign	w_BCD_in_y = {BCD_data_y[13], BCD_data_y[13], BCD_data_y[13], BCD_data_y[13], BCD_data_y[13], BCD_data_y[13], BCD_data_y[13], 
											 BCD_data_y[13], BCD_data_y[13], BCD_data_y[13], BCD_data_y[13], BCD_data_y[13], BCD_data_y[13:0]};
	assign	w_BCD_in_z = {BCD_data_z[13], BCD_data_z[13], BCD_data_z[13], BCD_data_z[13], BCD_data_z[13], BCD_data_z[13], BCD_data_z[13], 
											 BCD_data_z[13], BCD_data_z[13], BCD_data_z[13], BCD_data_z[13], BCD_data_z[13], BCD_data_z[13:0]};
	///////////////////////////////////////////////////////////////////////////////////////////
	/* ASCII�� ���� ������ ���̰� 33��Ʈ�� ��ȣ��Ʈ + 16��Ʈ�� ��ȯ = 17��Ʈ */
	assign	w_ASC_in_X	=	{w_BCD_DATA_X[32], w_BCD_DATA_X[15:0]};
	assign	w_ASC_in_Y	=	{w_BCD_DATA_Y[32], w_BCD_DATA_Y[15:0]};
	assign	w_ASC_in_Z	=	{w_BCD_DATA_Z[32], w_BCD_DATA_Z[15:0]};
	//////////////////////////////////////////////////////////////////////////////
	assign	O_txd = o_txd;			// RS-232��� (UART)�� Tx �� ���
	assign	oFSYNC = fsync;			// Frame sync�� 100Hz fsync �� ���
	
	
	
// ���� ���� ��, asd U1 (.U1��������/��°�(TOP����ǰ�))	
	i2c_4EA 		I2C_4EA			(.CLK(iCLK), .RST(iRST),
									 				 .sda_1(oSDA_1), .scl_1(oSCL_1),
									 			 	 .sda_2(oSDA_2), .scl_2(oSCL_2),
									 				 .sda_3(oSDA_3), .scl_3(oSCL_3),
									 				 .sda_4(oSDA_4), .scl_4(oSCL_4),
													 .sda_5(oSDA_5), .scl_5(oSCL_5),
													 .sda_6(oSDA_6), .scl_6(oSCL_6),
													 .sda_7(oSDA_7), .scl_7(oSCL_7),
													 .sda_8(oSDA_8), .scl_8(oSCL_8),
													 .i2c_start(fsync),				//i2c�� ���� ��ȣ, fsync�� ���߾� �� ���ý� i2c ������� ���ӵ� �����͸� ������.(ARM���� LCD�� ���� ����� ����)
									 				 .i2c_flag(i2c_flag),			//i2c_flag�� i2c ����� ���� �� ���� �������� ���� flag�� �� Ŭ�� ���� �߻���. ��� ����� EN�� ��� ����, BCD����� EN�� ��.
									 				 
									 				 .data_x_1(off_data_x_1), .data_y_1(off_data_y_1), .data_z_1(off_data_z_1), // ���� 6��Ʈ�� ���� 8��Ʈ�� 16��Ʈ ���·� �����.
									 			 	 .data_x_2(off_data_x_2), .data_y_2(off_data_y_2), .data_z_2(off_data_z_2),
									 				 .data_x_3(off_data_x_3), .data_y_3(off_data_y_3), .data_z_3(off_data_z_3),
									 				 .data_x_4(off_data_x_4), .data_y_4(off_data_y_4), .data_z_4(off_data_z_4),
													 .data_x_5(off_data_x_5), .data_y_5(off_data_y_5), .data_z_5(off_data_z_5),
													 .data_x_6(off_data_x_6), .data_y_6(off_data_y_6), .data_z_6(off_data_z_6),
													 .data_x_7(off_data_x_7), .data_y_7(off_data_y_7), .data_z_7(off_data_z_7),
													 .data_x_8(off_data_x_8), .data_y_8(off_data_y_8), .data_z_8(off_data_z_8)
													 );
									 // ���� 6��Ʈ�� ���� 8��Ʈ�� 16��Ʈ ���·� �����.
	
	
	
	OFF_ELI			OFFSET	(.CLK(iCLK), .RST(iRST),
									     .off_start(off_start),	//offset ���Ÿ� �����ϰ� �ϴ� Ű �Է�
											 .off_EN(i2c_flag),			//i2c���� �ѻ��� �������� ���� ������ ���� �˸��� i2c_flag�� �߻��ϸ� OFF_ELI��� Enable ��
											 .idata_x_1(off_data_x_1[15:2]), .idata_y_1(off_data_y_1[15:2]), .idata_z_1(off_data_z_1[15:2]),		//16bit�� ���� 6��Ʈ ���� 8��Ʈ 14bit�� ������^^
											 .idata_x_2(off_data_x_2[15:2]), .idata_y_2(off_data_y_2[15:2]), .idata_z_2(off_data_z_2[15:2]),		//16bit�� ���� 6��Ʈ ���� 8��Ʈ 14bit�� ������^^
											 .idata_x_3(off_data_x_3[15:2]), .idata_y_3(off_data_y_3[15:2]), .idata_z_3(off_data_z_3[15:2]),		//16bit�� ���� 6��Ʈ ���� 8��Ʈ 14bit�� ������^^
											 .idata_x_4(off_data_x_4[15:2]), .idata_y_4(off_data_y_4[15:2]), .idata_z_4(off_data_z_4[15:2]),		//16bit�� ���� 6��Ʈ ���� 8��Ʈ 14bit�� ������^^
											 .idata_x_5(off_data_x_5[15:2]), .idata_y_5(off_data_y_5[15:2]), .idata_z_5(off_data_z_5[15:2]),
											 .idata_x_6(off_data_x_6[15:2]), .idata_y_6(off_data_y_6[15:2]), .idata_z_6(off_data_z_6[15:2]),
											 .idata_x_7(off_data_x_7[15:2]), .idata_y_7(off_data_y_7[15:2]), .idata_z_7(off_data_z_7[15:2]),
											 .idata_x_8(off_data_x_8[15:2]), .idata_y_8(off_data_y_8[15:2]), .idata_z_8(off_data_z_8[15:2]),
											 .odata_x_1(SUM_data_x_1), .odata_y_1(SUM_data_y_1), .odata_z_1(SUM_data_z_1),		//14bit offset ���ŵ� x,y,z�� ���ӵ� ������(HPF ����ϴ� ȿ��)
											 .odata_x_2(SUM_data_x_2), .odata_y_2(SUM_data_y_2), .odata_z_2(SUM_data_z_2),		//14bit offset ���ŵ� x,y,z�� ���ӵ� ������(HPF ����ϴ� ȿ��)
											 .odata_x_3(SUM_data_x_3), .odata_y_3(SUM_data_y_3), .odata_z_3(SUM_data_z_3),		//14bit offset ���ŵ� x,y,z�� ���ӵ� ������(HPF ����ϴ� ȿ��)
											 .odata_x_4(SUM_data_x_4), .odata_y_4(SUM_data_y_4), .odata_z_4(SUM_data_z_4),		//14bit offset ���ŵ� x,y,z�� ���ӵ� ������(HPF ����ϴ� ȿ��)
											 .odata_x_5(SUM_data_x_5), .odata_y_5(SUM_data_y_5), .odata_z_5(SUM_data_z_5),
											 .odata_x_6(SUM_data_x_6), .odata_y_6(SUM_data_y_6), .odata_z_6(SUM_data_z_6),
											 .odata_x_7(SUM_data_x_7), .odata_y_7(SUM_data_y_7), .odata_z_7(SUM_data_z_7),
											 .odata_x_8(SUM_data_x_8), .odata_y_8(SUM_data_y_8), .odata_z_8(SUM_data_z_8),
											 .LED1(LED1), .LED2(LED2),	//Ű�Է°� offset ��� �ϷḦ ǥ���ϴ� LED ���
											 .off_flag(sum_flag));		//offset ���Ű� �ѻ��� �Ϸ���� ��Ÿ���� flag, LPF_FILTER�� Enable�� ������					 										 
	
									 				 
	SUM_DATA		sum_data		(.CLK(iCLK), .RST(iRST),
													 .In_XDATA_1(SUM_data_x_1), .In_YDATA_1(SUM_data_y_1), .In_ZDATA_1(SUM_data_z_1), 
										 			 .In_XDATA_2(SUM_data_x_2), .In_YDATA_2(SUM_data_y_2), .In_ZDATA_2(SUM_data_z_2),
													 .In_XDATA_3(SUM_data_x_3), .In_YDATA_3(SUM_data_y_3), .In_ZDATA_3(SUM_data_z_3),
													 .In_XDATA_4(SUM_data_x_4), .In_YDATA_4(SUM_data_y_4), .In_ZDATA_4(SUM_data_z_4),
													 .In_XDATA_5(SUM_data_x_5), .In_YDATA_5(SUM_data_y_5), .In_ZDATA_5(SUM_data_z_5),
													 .In_XDATA_6(SUM_data_x_6), .In_YDATA_6(SUM_data_y_6), .In_ZDATA_6(SUM_data_z_6),
													 .In_XDATA_7(SUM_data_x_7), .In_YDATA_7(SUM_data_y_7), .In_ZDATA_7(SUM_data_z_7),
													 .In_XDATA_8(SUM_data_x_8), .In_YDATA_8(SUM_data_y_8), .In_ZDATA_8(SUM_data_z_8),
													 .SUM_DATA_X(BCD_data_x), .SUM_DATA_Y(BCD_data_y), .SUM_DATA_Z(BCD_data_z),
													 .sum_flag(sum_flag), .bcd_flag(bcd_flag));
	
	TOP_NATFC		NATFC		(.CLK(iCLK), .RST(iRST), .ftick(bcd_flag), .in_data(BCD_data_z), .beep_out(beep_out));
	
	
	FRAME_SYNC	FRAME		(.CLK(iCLK), .RST(iRST),
											 .x_in_data(BCD_data_x), .y_in_data(BCD_data_y), .z_in_data(BCD_data_z),
											 .sclk(oSCLK), .fsync(fsync),
											 .x_out_data(oXOUT), .y_out_data(oYOUT), .z_out_data(oZOUT));
	wire BCD_flag;
	
	BCD_26bit		BCD_X		(.CLK(iCLK), .RST(iRST), .BCD_EN(bcd_flag), .bin_data(w_BCD_in_x), .oDATA(w_BCD_DATA_X));
	BCD_26bit		BCD_Y		(.CLK(iCLK), .RST(iRST), .BCD_EN(bcd_flag), .bin_data(w_BCD_in_y), .oDATA(w_BCD_DATA_Y));
	BCD_26bit		BCD_Z		(.CLK(iCLK), .RST(iRST), .BCD_EN(bcd_flag), .bin_data(w_BCD_in_z), .oDATA(w_BCD_DATA_Z), .BCD_flag(BCD_flag));

	
	ASCII		X_Value	(.CLK(iCLK), .RST(iRST), .iDATA(w_ASC_in_X), .oDATA(O_ASC_X));
	ASCII		Y_Value	(.CLK(iCLK), .RST(iRST), .iDATA(w_ASC_in_Y), .oDATA(O_ASC_Y));
	ASCII		Z_Value	(.CLK(iCLK), .RST(iRST), .iDATA(w_ASC_in_Z), .oDATA(O_ASC_Z));
	
	rs232c_clk	 RS232_CLK	(.CLK(iCLK), .RST(iRST), .Tx_en(Tx_en), .ifsync(BCD_flag));
	rs232c_tx 		RS232_TX	(.CLK(iCLK), .RST(iRST),
													 .iDATA_X(O_ASC_X), .iDATA_Y(O_ASC_Y), .iDATA_Z(O_ASC_Z),
													 .O_txd(o_txd), .Tx_en(Tx_en));
												
													 
	LCD	 LCD (.CLK(iCLK), .RST(iRST), .RS(LCD_RS), .Enable(LCD_EN), .RW(LCD_RW), .DATA(LCD_DATA));
	//LCD					LCD			(.CLK(iCLK), .RST(iRST), .LCD_DATA(LCD_DATA), .LCD_RW(LCD_RW), .LCD_EN(LCD_EN), .LCD_RS(LCD_RS));
						
	ARM_CLOCK		arm_clk			(.CLK(iCLK), .arm_clk(oARM_CLK));
							 
endmodule
													