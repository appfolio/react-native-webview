import React, {Component} from 'react';
import {View} from 'react-native';

import WebView from 'react-native-webview';

type Props = {};
type State = {};

export default class Alerts extends Component<Props, State> {
  state = {};

  render() {
    return (
      <View style={{ height: 300 }}>
        <WebView
          source={{ uri: 'https://www.7-zip.org/download.html' }}
          onShouldStartLoadWithRequest={(event) => {
            console.warn('event', event);
            // throw new Error('sjhit');
            // global.MagicNumber = 'lolol';
            console.warn('Is it there?', global.NativeSampleModule.getMagic());
            return true;
          }}
        />
      </View>
    );
  }
}
