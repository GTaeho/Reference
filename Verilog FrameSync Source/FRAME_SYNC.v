/*
  Frame Sync Design 
  Data transfer
*/

module FRAME_SYNC(CLK, RST,
                  x_in_data, y_in_data, z_in_data,
                  sclk, fsync,
                  x_out_data, y_out_data, z_out_data);

	/****** main Clock  (54Mhz) ******/
	input		CLK; 						
	///////////////////////////////////													
	input		RST;
	/* 14bit 가속도 센서 데이터 입력 */
	input		[13:0]	x_in_data;
	input 	[13:0]	y_in_data;
	input		[13:0]	z_in_data;
	///////////////////////////////////
	/******** transfer clock *********/
	output					sclk;															
	/********** fsync clock **********/
	output					fsync;							
	///////////////////////////////////
	output					x_out_data;
	output					y_out_data;
	output					z_out_data;


	parameter	TX_CNT	= 14;
	parameter HI = 1, LO = 0;
	
	// CLK = 54Mhz
	// 10kHz를 만들기 위해 카운터 : 2699 ->> {(54MHz/10KHz)/2}-1
	
	/************** 10Khz에 사용 되는 regiser ******************/
	reg		[19:0]	cnt_10k;
	reg						clk_10k;
	/////////////////////////////////////////////////////////////
	/************** 100Hz에 사용 되는 regiser ******************/
	reg		[9:0]		cnt;
	reg						r_fsync;
	/////////////////////////////////////////////////////////////
	reg	[TX_CNT:1]	x_buffer;		//[14:1]
	reg	[TX_CNT:1]	y_buffer;		//[14:1]
	reg	[TX_CNT:1]	z_buffer;		//[14:1]
	
	/********* 10KHz(sclk)를 만들기 위한 always 문 *********/
	always @ (posedge CLK or negedge RST)
		begin
			if(!RST)
				begin
					cnt_10k <= 0;
					clk_10k <= 1;
				end
			else
				begin
					if(cnt_10k == 2699)
						begin
							cnt_10k <= 0;
							clk_10k <= ~clk_10k;
						end
					else
						begin
							cnt_10k <= cnt_10k +1;
							clk_10k <= clk_10k;
						end
				end
		end		
		/******** sclk를 10KHz로 정의 ***********/
		assign sclk = clk_10k;
//////////////////////////////////////////////



/********* 100Hz(fsync)를 만들기 위한 always 문 *********/		
	always @ (negedge clk_10k or negedge RST)
		begin
			if(!RST)
				begin
					cnt 		<= 1;
					r_fsync <= 0;
				end
			else
				begin
					if(cnt == 100)
						begin
							r_fsync	<= 1;					// 100Hz 클럭 생성.
							cnt			<= 1;
						end
					else
						begin
							r_fsync	<= 0;
							cnt			<= cnt + 1;
						end
				end
		end
		/********** fsync 를 100Hz로 정의 ************/
		assign fsync	=	r_fsync;
		//////////////////////////////////////////////
		

		
		
		/****&&&&& testbench 용으로 삽입 &&&&*/
		
		always @ (posedge CLK or negedge RST)
			begin
				if(!RST)
					begin
						x_buffer <= 0;
						y_buffer <= 0;
						z_buffer <= 0;
					end
				else
					begin
						if(cnt == 1)
							begin
								x_buffer	<=	x_in_data;
								y_buffer	<=	y_in_data;
								z_buffer	<=	z_in_data;
							end					
						else
							begin
								x_buffer	<=	x_buffer;
								y_buffer	<=	y_buffer;
								z_buffer	<=	z_buffer;
							end
					end
			end
			
//internal val
//wire [TX_CNT:1] x_buffer =  x_in_data; 
//wire [TX_CNT:1] y_buffer =  y_in_data;
//wire [TX_CNT:1] z_buffer =  z_in_data;


	reg		[9:0]	tx_cnt = 0;
	reg					x_out_buf;
	reg					y_out_buf;
	reg					z_out_buf;
	
	always @ (posedge sclk or negedge RST)
		begin
			if(!RST)
				begin
					tx_cnt <= 0;
					x_out_buf <= 0;
					y_out_buf <= 0;
					z_out_buf <= 0;
				end
			else
				begin
					case(cnt)
						1,2,3,4,5,6,7,8,9,10,
						11,12,13,14 :
							begin
								x_out_buf <= x_buffer[TX_CNT - tx_cnt]; 
               	y_out_buf <= y_buffer[TX_CNT - tx_cnt]; 
              	z_out_buf <= z_buffer[TX_CNT - tx_cnt]; 
              	tx_cnt  <= tx_cnt +1;
							end
						default : //tx_cnt <= LO;
							begin
								x_out_buf <= LO;//x_buffer[TX_CNT - tx_cnt];
              	y_out_buf <= LO;//y_buffer[TX_CNT - tx_cnt]; 
               	z_out_buf <= LO;//z_buffer[TX_CNT - tx_cnt]; 
              	 tx_cnt  <= LO;
							end
					endcase
				end
		end
		
		assign x_out_data = x_out_buf;
		assign y_out_data = y_out_buf;
		assign z_out_data = z_out_buf;
  										
endmodule