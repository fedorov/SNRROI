#include "itkImageFileWriter.h"
#include "itkImageDuplicator.h"
#include "itkMetaDataObject.h"
#include "itkLevenbergMarquardtOptimizer.h"
#include "itkArray.h"


#include "itkPluginUtilities.h"

#include "SNRROICLP.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"

int main(int argc, char** argv){
  PARSE_ARGS;

  typedef itk::Image<float,3> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType> WriterType;
  typedef itk::ImageDuplicator<ImageType> DuplicatorType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageName.c_str());
  reader->Update();

  ImageType::Pointer inputImage = reader->GetOutput();

  DuplicatorType::Pointer dup = DuplicatorType::New();
  dup->SetInputImage(inputImage);
  dup->Update();
  ImageType::Pointer outputImage = dup->GetOutput();
  outputImage->FillBuffer(0);

  typedef itk::ImageRegionConstIteratorWithIndex<ImageType> InputIter;
  typedef itk::ImageRegionIteratorWithIndex<ImageType> OutputIter;
  typedef itk::ConstNeighborhoodIterator<ImageType> NeiIter;

  InputIter inIt(inputImage, inputImage->GetBufferedRegion());
  OutputIter outIt(outputImage, outputImage->GetBufferedRegion());

  NeiIter::SizeType neiRadius;
  neiRadius[0] = neiSize[0];
  neiRadius[1] = neiSize[1];
  neiRadius[2] = neiSize[2];

  unsigned numPixels = (neiSize[0]*2+1)*(neiRadius[1]*2+1)*(neiRadius[2]*2+1);
  NeiIter neiIter(neiRadius, inputImage, inputImage->GetBufferedRegion());


  //for(inIt.GoToBegin(),outIt.GoToBegin();!inIt.IsAtEnd();++inIt,++outIt){
  while(!neiIter.IsAtEnd()){
    std::vector<ImageType::PixelType> sample;
    float mean = 0, std = 0;
    for(int i=0;i<numPixels;i++){
      ImageType::IndexType idx = neiIter.GetIndex(i);
      bool isInBound = false;
      ImageType::PixelType p = neiIter.GetPixel(i, isInBound);
      mean += p;
      if(!isInBound)
        break;
      sample.push_back(p);
    }
    float snr = 0;
    mean /= sample.size();

    ImageType::IndexType centerIdx = neiIter.GetIndex(numPixels/2+1);
    if(sample.size()<numPixels) {
      inputImage->SetPixel(centerIdx, snr);
    } else {
      for(int i=0;i<numPixels;i++){
        //std::cout << sample[i] << " ";
        std += (mean-sample[i])*(mean-sample[i]);
      }
      //std::cout << std::endl;
      std = sqrt(std/numPixels);
      snr = mean/std;
      //std::cout << mean << " " << std << " " << snr << std::endl;
      outputImage->SetPixel(centerIdx, snr);
    }
    ++neiIter;
  }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(outputImage);
  writer->SetFileName(outputImageName.c_str());
  writer->Update();

  return 0;
}
