import React, { useState, useEffect } from 'react';

const Stream = (props) => {
    const { isStreamingActive } = props;
    const [imageSrc, setImageSrc] = useState('');
    const [lastImageInterval, setLastImageInterval] = useState(null);
    useEffect(() => {
        if (isStreamingActive) {
            setLastImageInterval(setInterval(async () => {
                try {
                    const response = await fetch('http://localhost:8000/last_image');
                    const data = await response.blob();
                    const imageUrl = URL.createObjectURL(data);
                    setImageSrc(imageUrl);
                } catch (error) {
                    console.error('Error fetching image:', error);
                }
            }
            , 1000));
        }else{
            clearInterval(lastImageInterval);
            setLastImageInterval(null);
            setImageSrc('');
        }
    }, [isStreamingActive]);

    return (
        <div>
            {imageSrc && <img src={imageSrc} alt="Last Image" />}
        </div>
    );
};

export default Stream;