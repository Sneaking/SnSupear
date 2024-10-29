import express from 'express';
import { FileController } from '../controllers/FileController.js';

const router = express.Router();
const fileController = new FileController();

router.post('/load', fileController.loadFile);
router.post('/save', fileController.saveFile);
router.get('/content/:id', fileController.getFileContent);

export const fileRouter = router;