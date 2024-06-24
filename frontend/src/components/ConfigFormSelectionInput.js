import React from 'react';
import { Form } from 'react-bootstrap';

const ConfigFormSelectionInput = ({ configs, setConfigs, section, sectionKey }) => {
    return (
        <Form.Control
            onChange={(e) =>
                setConfigs({
                    ...configs,
                    [section]: {
                        ...configs[section],
                        keys: {
                            ...configs[section].keys,
                            [sectionKey]: {
                                ...configs[section].keys[sectionKey],
                                value: e.target.value,
                            },
                        },
                    },
                })
            }
            as="select"
            value={configs[section].keys[sectionKey].value}
        >
            {configs[section]["keys"][sectionKey]["options"].map((option) => (
                <option
                    key={option}
                    value={option}
                >
                    {option}
                </option>
            ))}
        </Form.Control>
    );
};

export default ConfigFormSelectionInput;