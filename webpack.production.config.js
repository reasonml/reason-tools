const path = require('path');
const webpack = require('webpack');
const config = require('./webpack.config');
const ZipPlugin = require('zip-webpack-plugin');


const prodConfig = config.map((config) => {
  if (!config.plugins) {
    config.plugins = [];
  }
  config.plugins = config.plugins.concat([
    new webpack.DefinePlugin({
      'process.env':{
        'NODE_ENV': JSON.stringify('production')
      }
    }),
    new webpack.optimize.UglifyJsPlugin(),
    new webpack.optimize.DedupePlugin(),
    new webpack.optimize.AggressiveMergingPlugin(),
  ]);
  if (config.generateZip) {
    config.plugins = config.plugins.concat([
      new ZipPlugin({
        filename: 'reason-tools.zip',
      })
    ]);
  }
  return config;
});


module.exports = prodConfig;
