const webpack = require('webpack');

module.exports = {
  entry: {
    ContentSelectionText: './src/ContentSelectionText.js',
    Popup: './src/Popup.js',
    Background: './src/Background.js',
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
      },
      {
        test: /\.(png|jpg|gif|json|html)$/,
        loader: "file-loader?name=[name].[ext]"
      }
    ],
  },
  plugins: [
    // new webpack.DefinePlugin({
    //   'process.env':{
    //     'NODE_ENV': JSON.stringify('production')
    //   }
    // }),
    // new webpack.optimize.UglifyJsPlugin({
    // })
  ],
};
