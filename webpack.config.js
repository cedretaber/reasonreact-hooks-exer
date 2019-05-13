const path = require("path");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const outputDir = path.join(__dirname, "build/");

const isProd = process.env.NODE_ENV === "production";

module.exports = {
  entry: {
    Index: "./src/Index.bs.js",
    sw: "./src/sw.ts"
  },
  devtool: "inline-source-map",
  mode: isProd ? "production" : "development",
  output: {
    path: outputDir,
    filename: "[name].js"
  },
  module: {
    rules: [
      {
        test: /\.tsx?$/,
        use: "ts-loader",
        exclude: /node_modules/
      }
    ]
  },
  resolve: {
    extensions: [".tsx", ".ts", ".js"]
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: "src/index.html",
      inject: false
    })
  ],
  devServer: {
    compress: true,
    contentBase: outputDir,
    port: process.env.PORT || 8000,
    historyApiFallback: true,
    before: (app, _server) => {
      app.get(/^(?!\/api).*(?<!\.(?:js|css))$/, (_req, res) => {
        res.sendFile(path.join(__dirname, "build/index.html"));
      });
    }
  }
};
