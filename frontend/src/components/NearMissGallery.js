import React, { useState,useEffect } from 'react';
import { Container, Row, Col, Image, Modal, Button } from 'react-bootstrap';

const getThumbnailAddress = (key) => {
    // Return the thumbnail address based on the key
    return `http://localhost:8000/thumbnails/${key}`;
};

const getBigImageAddress = (key) => {
    // Return the big image address based on the key
    return `http://localhost:8000/images/${key}`;
};

const NearMissGallery = () => {
    const [allNearMissKeys, setAllNearMissKeys] = useState([]);
    const [visibleNearMissKeys, setVisibleNearMissKeys] = useState([]);
    const [showModal, setShowModal] = useState(false);
    const [currentImage, setCurrentImage] = useState('');

    useEffect(() => {
        fetch('http://localhost:8000/thumbnails')
            .then(response => response.json())
            .then(keys => {
                setAllNearMissKeys(keys)
                setVisibleNearMissKeys(keys.slice(0, 12));
            })
            .catch(error => console.error('Error fetching thumbnails:', error));
        }, []);

    const handleThumbnailClick = (key) => {
        setCurrentImage(getBigImageAddress(key));
        setShowModal(true);
    };

    const handleCloseModal = () => {
        setShowModal(false);
        setCurrentImage('');
    };
    const loadMore = () => {
        setVisibleNearMissKeys(allNearMissKeys.slice(0, visibleNearMissKeys.length + 12));
    }

    return (
        <Container className="gallery-container">
            <Row>
                {visibleNearMissKeys.map((key, index) => (
                    <Col xs={12} sm={6} md={4} lg={3} key={index} className="gallery-item">
                        <Image
                            src={getThumbnailAddress(key)}
                            alt={`Thumbnail ${key}`}
                            thumbnail
                            onClick={() => handleThumbnailClick(key)}
                        />
                    </Col>
                ))}
            </Row>
            { visibleNearMissKeys.length !== allNearMissKeys.length &&
                <Row>
                    <Col>
                        <Button variant="primary" onClick={loadMore}>Load More</Button>
                    </Col>
                </Row>
            }

            <Modal show={showModal} onHide={handleCloseModal} centered>
                <Modal.Body>
                    <Image src={currentImage} alt="Big Image" fluid />
                </Modal.Body>
            </Modal>
        </Container>
    );
};

export default NearMissGallery;
