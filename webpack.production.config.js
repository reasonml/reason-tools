const webpack = require('webpack');
const config = require('./webpack.config');

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

module.exports = config;
