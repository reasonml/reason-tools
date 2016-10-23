const webpack = require('webpack');

module.exports = {
  entry: {
    App: './src/App.js',
    Popup: './src/Popup.js',
  },
  output: {
    path: './_build/extension/',
    filename: '[name].bundle.js',
  },
  module: {
    loaders: [
      {
        test: /\.js$/,
        exclude: /(node_modules|_build)/,
        loader: 'babel-loader',
        query: {
          presets: ['latest', 'react', 'stage-0']
        }
      }
    ],
  },
  plugins: [
    // new webpack.optimize.UglifyJsPlugin()
  ],
};
