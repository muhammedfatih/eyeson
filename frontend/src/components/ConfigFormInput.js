import React from 'react';
import { Form } from 'react-bootstrap';

const ConfigFormInput = ({ configs, setConfigs, section, sectionKey, type = "text" }) => {
    const handleChange = (e) => {
        const updatedConfigs = {
            ...configs,
            [section]: {
                ...configs[section],
                keys: {
                    ...configs[section].keys,
                    [sectionKey]: {
                        ...configs[section].keys[sectionKey],
                        value: e.target.value
                    }
                }
            }
        };
        setConfigs(updatedConfigs);
    };

    return (
        <Form.Control
            onChange={handleChange}
            className={`${section} input`}
            type={type}
            defaultValue={configs[section].keys[sectionKey].value}
        />
    );
};

export default ConfigFormInput;