#include <Crawler/WorkerManager.hpp>
#include <Crawler/Application.hpp>
#include <Crawler/ScopedMutex.hpp>

Crawler::WorkerManager::WorkerManager ( Application & application ) :
	application ( application )
{
}
			
Crawler::Application & Crawler::WorkerManager::getApplication ( )
{
	return this->application ;
}
			
const Crawler::Application & Crawler::WorkerManager::getApplication ( ) const
{
	return this->application ;
}

void Crawler::WorkerManager::setAmountOfWorker ( std::size_t amountOfWorker )
{
	Crawler::ScopedMutex mutex ( this->mutex ) ;
	this->amountOfWorker = amountOfWorker ;
	
	if ( this->worker.size ( ) > 0 )
	{
		this->despawnWorker ( ) ;
		this->spawnWorker ( ) ;
	}
}
			
std::size_t Crawler::WorkerManager::getAmountOfWorker ( ) const
{
	return this->amountOfWorker ;
}
			
void Crawler::WorkerManager::spawnWorker ( )
{
	Crawler::ScopedMutex mutex ( this->mutex ) ;
	for ( std::size_t i = 0 ; i < this->amountOfWorker ; ++i )
	{
		this->worker.push_back ( std::unique_ptr <Crawler::Worker> ( new Crawler::Worker ( * this ) ) ) ;
		this->worker.back ( )->launch ( ) ;
		
		this->getApplication ( ).onSpawnWorker ( * this->worker.back ( ) ) ;
	}
}
			
void Crawler::WorkerManager::despawnWorker ( )
{
	Crawler::ScopedMutex mutex ( this->mutex ) ;
	for ( auto iterator = this->worker.begin ( ) ; iterator != this->worker.end ( ) ; ++iterator )
	{
		this->getApplication ( ).onDespawnWorker ( ** iterator ) ;
	
		( * iterator )->terminate ( ) ;
	}
	
	this->worker.clear ( ) ;
}

const std::list <std::unique_ptr <Crawler::Worker>> & Crawler::WorkerManager::getWorker ( ) const
{
	return this->worker ;
}

Crawler::WorkerManager::ConstIterator Crawler::WorkerManager::begin ( ) const
{
	return this->worker.begin ( ) ;
}
Crawler::WorkerManager::ConstIterator Crawler::WorkerManager::cbegin ( ) const
{
	return this->worker.cbegin ( ) ;
}
Crawler::WorkerManager::ConstIterator Crawler::WorkerManager::end ( ) const
{
	return this->worker.end ( ) ;
}
Crawler::WorkerManager::ConstIterator Crawler::WorkerManager::cend ( ) const
{
	return this->worker.cend ( ) ;
}
