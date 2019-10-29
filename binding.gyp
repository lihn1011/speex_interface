{
  "targets": [
    {
      "target_name": "speex_interface",
      "sources": [
                "./cpp/speex_interface.cpp", 
                './cpp/speex_1.2.0/libspeex/bits.c',
                './cpp/speex_1.2.0/libspeex/cb_search.c',
                './cpp/speex_1.2.0/libspeex/exc_5_64_table.c',
                './cpp/speex_1.2.0/libspeex/exc_5_256_table.c',
                './cpp/speex_1.2.0/libspeex/exc_8_128_table.c',
                './cpp/speex_1.2.0/libspeex/exc_10_16_table.c',
                './cpp/speex_1.2.0/libspeex/exc_10_32_table.c',
                './cpp/speex_1.2.0/libspeex/exc_20_32_table.c',
                './cpp/speex_1.2.0/libspeex/filters.c',
                './cpp/speex_1.2.0/libspeex/gain_table.c',
                './cpp/speex_1.2.0/libspeex/gain_table_lbr.c',
                './cpp/speex_1.2.0/libspeex/hexc_10_32_table.c',
                './cpp/speex_1.2.0/libspeex/hexc_table.c',
                './cpp/speex_1.2.0/libspeex/high_lsp_tables.c',
                './cpp/speex_1.2.0/libspeex/kiss_fft.c',
                './cpp/speex_1.2.0/libspeex/kiss_fftr.c',
                './cpp/speex_1.2.0/libspeex/lpc.c',
                './cpp/speex_1.2.0/libspeex/lsp.c',
                './cpp/speex_1.2.0/libspeex/lsp_tables_nb.c',
                './cpp/speex_1.2.0/libspeex/ltp.c',
                './cpp/speex_1.2.0/libspeex/modes.c',
                './cpp/speex_1.2.0/libspeex/modes_wb.c',
                './cpp/speex_1.2.0/libspeex/nb_celp.c',
                './cpp/speex_1.2.0/libspeex/quant_lsp.c',
                './cpp/speex_1.2.0/libspeex/smallft.c',
                './cpp/speex_1.2.0/libspeex/speex.c',
                './cpp/speex_1.2.0/libspeex/speex_callbacks.c',
                './cpp/speex_1.2.0/libspeex/speex_header.c',
                './cpp/speex_1.2.0/libspeex/stereo.c',
                './cpp/speex_1.2.0/libspeex/vbr.c',
                './cpp/speex_1.2.0/libspeex/vq.c',
                './cpp/speex_1.2.0/libspeex/window.c',
                './cpp/speex_1.2.0/libspeex/sb_celp.c'
      ],
      'cflags': [
                      '-pthread',
                      '-fno-exceptions',
                      '-fno-strict-aliasing',
                      '-Wall',
                      '-Wno-unused-parameter',
                      '-Wno-missing-field-initializers',
                      '-Wextra',
                      '-pipe',
                      '-fno-ident',
                      '-fdata-sections',
                      '-ffunction-sections',
                      '-fPIC'
                  ],
                  'defines': [
                   'FLOATING_POINT',
    'USE_KISS_FFT',
    'EXPORT='
                  ],
                'include_dirs': [
                    './cpp/speex_1.2.0/include',
                    "<!(node -e \"require('nan')\")"
                ],
              }
  ]
}