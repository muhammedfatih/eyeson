import Stream from './components/Stream';
import ConfigForm from './components/ConfigForm';
import NearMissGallery from './components/NearMissGallery';
import { Container, Tabs, Tab } from 'react-bootstrap';
import { useState } from 'react';

function App() {
  const [isStreamingActive, setIsStreamingActive] = useState(false);

  const handleTabClick = (key) => {
    setIsStreamingActive(key.target.dataset.rrUiEventKey === 'STREAM');
  }

  return (
    <div className="App">
      <h1>Eyes On Interface</h1>
      <hr/>
      <Container>
        <Tabs defaultActiveKey="NEAR_MISS" id="uncontrolled-tab-example" onClick={(e) => handleTabClick(e)} >
          <Tab eventKey="NEAR_MISS" title="Near Miss Gallery">
            <NearMissGallery />
          </Tab>
          <Tab eventKey="CONFIG" title="Configuration">
            <ConfigForm />
          </Tab>
          <Tab eventKey="STREAM" title="Stream">
            <Stream isStreamingActive={isStreamingActive} />
          </Tab>
        </Tabs>
      </Container>
    </div>
  );
}

export default App;