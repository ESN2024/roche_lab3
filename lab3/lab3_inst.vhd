	component lab3 is
		port (
			clk_clk                             : in    std_logic                    := 'X'; -- clk
			opencores_i2c_0_export_0_scl_pad_io : inout std_logic                    := 'X'; -- scl_pad_io
			opencores_i2c_0_export_0_sda_pad_io : inout std_logic                    := 'X'; -- sda_pad_io
			pio_0_external_connection_export    : out   std_logic_vector(7 downto 0);        -- export
			reset_reset_n                       : in    std_logic                    := 'X'  -- reset_n
		);
	end component lab3;

	u0 : component lab3
		port map (
			clk_clk                             => CONNECTED_TO_clk_clk,                             --                       clk.clk
			opencores_i2c_0_export_0_scl_pad_io => CONNECTED_TO_opencores_i2c_0_export_0_scl_pad_io, --  opencores_i2c_0_export_0.scl_pad_io
			opencores_i2c_0_export_0_sda_pad_io => CONNECTED_TO_opencores_i2c_0_export_0_sda_pad_io, --                          .sda_pad_io
			pio_0_external_connection_export    => CONNECTED_TO_pio_0_external_connection_export,    -- pio_0_external_connection.export
			reset_reset_n                       => CONNECTED_TO_reset_reset_n                        --                     reset.reset_n
		);

