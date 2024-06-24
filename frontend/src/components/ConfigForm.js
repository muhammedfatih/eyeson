import React, {useEffect, useState} from 'react';
import { Container, Tabs, Tab, Row, Col, Card, Form, Button } from 'react-bootstrap';
import ConfigFormInput from './ConfigFormInput';
import ConfigFormSelectionInput from './ConfigFormSelectionInput';

const ConfigForm = () => {
    const [configs, setConfigs] = useState([]);
    useEffect(() => {
      fetch('http://localhost:8000/configs')
        .then(response => response.json())
        .then(data => setConfigs(data));
    }, []);

    const saveConfig = (configKey) => {
        let sectionKeys = configs[configKey]["keys"];
        Object.keys(sectionKeys).forEach((key) => {
            let sectionKey = sectionKeys[key];
            fetch(`http://localhost:8000/config/${configKey}/${sectionKey.key}`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({value: sectionKey["value"]})
            })
        })
    }
    return (
        <div className="App">
            <Container>
                <Tabs defaultActiveKey={Object.keys(configs)[0]}>
                    {Object.keys(configs).map((section) => (
                        <Tab key={section} eventKey={section} title={configs[section]["label"]}>
                            <Row>
                                <Col>
                                    <Card>
                                        <Card.Body>
                                            <Card.Title>{configs[section]["label"]}</Card.Title>
                                            <Form>
                                                {Object.keys(configs[section]["keys"]).map((key) => (
                                                    <Form.Group key={key} controlId={key}>
                                                        <Form.Label>{configs[section]["keys"][key]["label"]}</Form.Label>
                                                        {["text", "number"].includes(configs[section]["keys"][key]["type"]) &&
                                                            <ConfigFormInput configs={configs} setConfigs={setConfigs} section={section} sectionKey={key} type={configs[section]["keys"][key]["type"]} />
                                                        }
                                                        {configs[section]["keys"][key]["type"] === "coconames" &&
                                                            <ConfigFormSelectionInput configs={configs} setConfigs={setConfigs} section={section} sectionKey={key} />
                                                        }
                                                    </Form.Group>
                                                ))}
                                                <Button onClick={() => saveConfig(section)} variant="primary">Save</Button>
                                            </Form>
                                        </Card.Body>
                                    </Card>
                                </Col>
                            </Row>
                        </Tab>
                    ))}
                </Tabs>
            </Container>
        </div>
    );
};

export default ConfigForm;