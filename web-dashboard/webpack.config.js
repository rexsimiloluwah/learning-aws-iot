module.exports = {
    entry: './main.js',
    mode: 'development',
    output: {
      path: `${__dirname}/dist`,
      filename: 'main.js',
    },
    module: {
        noParse: [/aws\-sdk/]
    },
  };